#include "GhostManager.h"
#include "UnitUtil.h"

using namespace UAlbertaBot;

GhostManager::GhostManager() 
{ 
	
}

void GhostManager::executeMicro(const BWAPI::Unitset & targets)
{
	const BWAPI::Unitset & ghosts = getUnits();

	// figure out targets
	BWAPI::Unitset ghostTargets;
	std::copy_if(targets.begin(), targets.end(), std::inserter(ghostTargets, ghostTargets.end()),
		[](BWAPI::Unit u){ return u->isVisible();  });

	//set the range of the ghost
	int ghostRange = BWAPI::UnitTypes::Terran_Ghost.groundWeapon().maxRange() - 16;
	//set energy requirements
	int cloakingEnergy = BWAPI::UnitTypes::Terran_Ghost.cloakingTech().energyCost;
	int lockdownEnergy = BWAPI::TechTypes::Lockdown.energyCost();


	for (auto & ghost : ghosts)
	{
		bool ghostNearEnemy = false;
		if (order.getType() == SquadOrderTypes::Attack)
		{
			// if attacking, do not attack units

			// find the best target for this tank
			BWAPI::Unit target = getTarget(ghost, ghostTargets);
			if (!targets.empty()){

				
				// cloak
				if (ghost->getDistance(target) < 2 * ghostRange && !ghost->isCloaked && ghost->canCloak() && ghost->getEnergy > 1.5*cloakingEnergy){
					ghostNearEnemy = true;
					ghost->cloak();
				}

				//lockdown enemy carriers
				if (target->getType == BWAPI::UnitTypes::Protoss_Carrier && !target->isLockedDown() && ghost->getEnergy > lockdownEnergy + cloakingEnergy){
					//maybe check if the tech has been researched?
					ghost->useTech(BWAPI::TechTypes::Lockdown, target);
				}

				// if building but not mineral or geyser, and nuke is available: paint nuke target

			}
		}
		else if (order.getType() == SquadOrderTypes::Defend)
		{
			// find the best target for this tank
			BWAPI::Unit target = getTarget(ghost, ghostTargets);

			//for all targets
			// if target is carrier, lock it down
			// if target is battlecruiser, lock it down
			// if target is tank, lock it down
			// else, kill it
		}
	}
}





/*

nuking implentation:
-wait until nuke is ready <-- in CombatManager.cpp?
-move to enemy base
-cloke if enemy is in range
-find building target
-launch nuke

anti-carrier implementation
-find carrier target
-lockdown
-shoot carrier
-refresh lockdown if it's not dead. 

*/


// get a target for the ghost to attack
BWAPI::Unit GhostManager::getTarget(BWAPI::Unit ghost, const BWAPI::Unitset & targets)
{
	int bestPriorityDistance = 1000000;
	int bestPriority = 0;

	double bestLTD = 0;

	BWAPI::Unit bestTargetThreatInRange = nullptr;
	double bestTargetThreatInRangeLTD = 0;

	int highPriority = 0;
	double closestDist = std::numeric_limits<double>::infinity();
	BWAPI::Unit closestTarget = nullptr;

	int ghostRange = BWAPI::UnitTypes::Terran_Ghost.groundWeapon().maxRange() - 16;
	BWAPI::Unitset targetsInRange;
	for (auto & target : targets)
	{
		if (target->getDistance(ghost) < ghostRange && UnitUtil::CanAttack(ghost, target))
		{
			targetsInRange.insert(target);
		}
	}

	const BWAPI::Unitset & newTargets = targetsInRange.empty() ? targets : targetsInRange;

	// check first for units that are in range of our attack that can cause damage
	// choose the highest priority one from them at the lowest health
	for (const auto & target : newTargets)
	{
		if (!UnitUtil::CanAttack(ghost, target))
		{
			continue;
		}

		double distance = ghost->getDistance(target);
		double LTD = UnitUtil::CalculateLTD(target, ghost);
		int priority = getAttackPriority(ghost, target);
		bool targetIsThreat = LTD > 0;
		BWAPI::Broodwar->drawTextMap(target->getPosition(), "%d", priority);

		if (!closestTarget || (priority > highPriority) || (priority == highPriority && distance < closestDist))
		{
			closestDist = distance;
			highPriority = priority;
			closestTarget = target;
		}
	}

	if (bestTargetThreatInRange)
	{
		return bestTargetThreatInRange;
	}

	return closestTarget;
}

// get the attack priority of a type in relation to a zergling
int GhostManager::getAttackPriority(BWAPI::Unit rangedUnit, BWAPI::Unit target)
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
	else if (targetType == BWAPI::UnitTypes::Protoss_Pylon)
	{
		return 8;
	}
	// next is special buildings
	else if (targetType == BWAPI::UnitTypes::Zerg_Spawning_Pool)
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

BWAPI::Unit GhostManager::closestrangedUnit(BWAPI::Unit target, std::set<BWAPI::Unit> & rangedUnitsToAssign)
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