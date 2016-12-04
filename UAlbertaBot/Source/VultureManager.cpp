#include "VultureManager.h"
#include "UnitUtil.h"

using namespace UAlbertaBot;
//optimised for harrassment modified from TankManager
VultureManager::VultureManager()
{
}

void VultureManager::executeMicro(const BWAPI::Unitset & targets)
{
	const BWAPI::Unitset & vultures = getUnits();

	// figure out targets
	BWAPI::Unitset vultureTargets;
	std::copy_if(targets.begin(), targets.end(), std::inserter(vultureTargets, vultureTargets.end()),
		[](BWAPI::Unit u){ return u->isVisible() && !u->isFlying(); });

//expand to find number
	bool haveSpider = BWAPI::Broodwar->self()->hasResearched(BWAPI::TechTypes::Spider_Mines);



	// for each vulture
	for (auto & vulture : vultures)
	{

		bool vultureNearChokepoint = false;
		BWTA::Region * enemyBaseLocation = InformationManager::Instance().getMainBaseLocation(BWAPI::Broodwar->enemy())->getRegion();
		
		
		for (auto & choke : enemyBaseLocation->getChokepoints())
		{
			if (choke->getCenter().getDistance(vulture->getPosition()) < 192)
			{
				vultureNearChokepoint = true;
				BWAPI::Broodwar->drawCircleMap(choke->getCenter(), 192, BWAPI::Colors::Green);
				break;
			}
		}
		
		// if the order is to attack or defend
		if (order.getType() == SquadOrderTypes::Attack || order.getType() == SquadOrderTypes::Defend)
		{
			// if there are targets
			if (!vultureTargets.empty())
			{
				// find the best target for this vulture
				BWAPI::Unit target = getTarget(vulture, vultureTargets);

				if (target && Config::Debug::DrawUnitTargetInfo)
				{
					BWAPI::Broodwar->drawLineMap(vulture->getPosition(), vulture->getTargetPosition(), BWAPI::Colors::Purple);
				}
				
				int tType = target->getType();
				if ((tType == BWAPI::UnitTypes::Protoss_Photon_Cannon || tType == BWAPI::UnitTypes::Zerg_Sunken_Colony /*|| tType == BWAPI::UnitTypes::Terran_Bunker*/)
				&& vulture->getDistance(target) < target->getType().groundWeapon().maxRange() && targets.size() <=1){
					Micro::SmartMove(vulture, enemyBaseLocation->getCenter());
				}



				//buildings and workers have priority 5 or less
				//dont waste time on them; target workers
				if (getAttackPriority(vulture, target) < 6){
					//if we have mines and are targeting a worker bomb it
					if (target->getType().isWorker() && haveSpider && vulture->getSpiderMineCount() > 0){
						Micro::SmartLaySpiderMine(vulture, target->getPosition());
					}else{
						Micro::SmartAttackUnit(vulture, target);
					}
				}
				else{
					// kite the target
					Micro::MutaDanceTarget(vulture, target);
				}
			}
			else
				// if there are no targets
			{
//				probably causes units to get stuck by chokepoints
				int smc = vulture->getSpiderMineCount();
				if (haveSpider && vulture->getSpiderMineCount() > 2 && vultureNearChokepoint)
				{
					Micro::SmartLaySpiderMine(vulture, vulture->getPosition());
				} 

				// if we're not near the order position
				else if(vulture->getDistance(order.getPosition()) > 100)
				{
					// move to it
					Micro::SmartAttackMove(vulture, order.getPosition());
				}
			}
		}
		else if (order.getType() == SquadOrderTypes::Regroup && haveSpider){
			Micro::SmartLaySpiderMine(vulture, order.getPosition());
		}
	}
}

// get a target for the vulture to attack
BWAPI::Unit VultureManager::getTarget(BWAPI::Unit vulture, const BWAPI::Unitset & targets)
{
	int bestPriorityDistance = 1000000;
	int bestPriority = 0;

	double bestLTD = 0;

	BWAPI::Unit bestTargetThreatInRange = nullptr;
	double bestTargetThreatInRangeLTD = 0;

	int highPriority = 0;
	double closestDist = std::numeric_limits<double>::infinity();
	BWAPI::Unit closestTarget = nullptr;
	

	// check first for units that are in range of our attack that can cause damage
	// choose the highest priority one from them at the lowest health
	for (const auto & target : targets)
	{
		if (!UnitUtil::CanAttack(vulture, target))
		{
			continue;
		}

		double distance = vulture->getDistance(target);
		double LTD = UnitUtil::CalculateLTD(target, vulture);
		int priority = getAttackPriority(vulture, target);
		bool targetIsThreat = LTD > 0;
		BWAPI::Broodwar->drawTextMap(target->getPosition(), "%d", priority);

		if (!closestTarget || (targetIsThreat && distance < closestDist))
		{
			closestDist = distance;
			highPriority = priority;
			closestTarget = target;
		}
	}

	return closestTarget;
}

// get the attack priority of a type in relation to a zergling
//should not get called because harrass kites the closest
int VultureManager::getAttackPriority(BWAPI::Unit rangedUnit, BWAPI::Unit target)
{
	BWAPI::UnitType rangedType = rangedUnit->getType();
	BWAPI::UnitType targetType = target->getType();

	bool isThreat = rangedType.isFlyer() ? targetType.airWeapon() != BWAPI::WeaponTypes::None : targetType.groundWeapon() != BWAPI::WeaponTypes::None;

	if (target->getType().isWorker())
	{
		isThreat = false;
	}

	if (target->getType() == BWAPI::UnitTypes::Zerg_Larva || target->getType() == BWAPI::UnitTypes::Zerg_Egg)
	{
		return 0;
	}

	// if the target is building something near our base something is fishy
	BWAPI::Position ourBasePosition = BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation());
	if (target->getType().isWorker() && (target->isConstructing() || target->isRepairing()) && target->getDistance(ourBasePosition) < 1200)
	{
		return 100;
	}

	if (target->getType().isBuilding() && (target->isCompleted() || target->isBeingConstructed()) && target->getDistance(ourBasePosition) < 1200)
	{
		return 90;
	}

	// highest priority is something that can attack us or aid in combat
	if (targetType == BWAPI::UnitTypes::Terran_Bunker || isThreat)
	{
		return 11;
	}
	// next priority is worker
	else if (targetType.isWorker())
	{
		return 9;
	}
	// next is special buildings
	else if (targetType == BWAPI::UnitTypes::Zerg_Spawning_Pool)
	{
		return 5;
	}
	// next is special buildings
	else if (targetType == BWAPI::UnitTypes::Protoss_Pylon)
	{
		return 5;
	}
	// next is buildings that cost gas
	else if (targetType.gasPrice() > 0)
	{
		return 4;
	}
	else if (targetType.mineralPrice() > 0)
	{
		return 3;
	}
	// then everything else
	else
	{
		return 1;
	}
}

BWAPI::Unit VultureManager::closestrangedUnit(BWAPI::Unit target, std::set<BWAPI::Unit> & rangedUnitsToAssign)
{
	double minDistance = 0;
	BWAPI::Unit closest = nullptr;

	for (auto & rangedUnit : rangedUnitsToAssign)
	{
		double distance = rangedUnit->getDistance(target);
		if (!closest || distance < minDistance)
		{
			minDistance = distance;
			closest = rangedUnit;
		}
	}

	return closest;
}

//time to override a non virtual function
//check squad.cpp to see if this is ever used
void VultureManager::regroup(const BWAPI::Position & regroupPosition) const
{
	BWAPI::Position ourBasePosition = BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation());
	int regroupDistanceFromBase = MapTools::Instance().getGroundDistance(regroupPosition, ourBasePosition);

BWAPI::Broodwar->drawCircleMap(regroupPosition, 10, BWAPI::Colors::Orange);
BWAPI::Broodwar->setScreenPosition(regroupPosition - BWAPI::Position(320, 180));

	const BWAPI::Unitset & vultures = getUnits();
	bool haveSpider = BWAPI::Broodwar->self()->hasResearched(BWAPI::TechTypes::Spider_Mines);

	// for each of the units we have
	for (auto & vulture : vultures)
	{
		int unitDistanceFromBase = MapTools::Instance().getGroundDistance(vulture->getPosition(), ourBasePosition);

		// if the unit is outside the regroup area
		if (unitDistanceFromBase > regroupDistanceFromBase)
		{
			Micro::SmartMove(vulture, ourBasePosition);
		}
		else if (vulture->getDistance(regroupPosition) > 100)
		{
			if (haveSpider){
				Micro::SmartLaySpiderMine(vulture, regroupPosition);
			}
			else {
				// regroup it
				Micro::SmartMove(vulture, regroupPosition);
			}
			
		}
		else
		{
			Micro::SmartAttackMove(vulture, vulture->getPosition());
		}
	}
}
