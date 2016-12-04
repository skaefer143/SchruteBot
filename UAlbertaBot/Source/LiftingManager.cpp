#include "LiftingManager.h"
#include "ProductionManager.h"

using namespace UAlbertaBot;

LiftingManager & LiftingManager::Instance()
{
	static LiftingManager instance;
	return instance;
}

void LiftingManager::update(){
	//check if part of the wall needs to be lifted, to let troops through.
	if (ProductionManager::Instance().getMadeFirstWall()){
		LiftingManager liftMan = LiftingManager::Instance();
		UnitData myUnitData = InformationManager::Instance().getUnitData(BWAPI::Broodwar->self());
		std::vector<Building> myBuildings = BuildingManager::Instance().buildingsWeHaveBuilt;
		for (Building & b : myBuildings){
			if (b.isPartOfWall){
				if (b.buildingUnit->isLifted()){
					liftMan.checkForSetDown(b);
				}
				else if (b.buildingUnit->isCompleted() && b.isPartOfWall && !b.buildingUnit->isLifted()
					&& b.buildingUnit->getType().getRace() == BWAPI::Races::Terran){
					//lift wall, in lifting manager
					liftMan.checkForLiftOff(b);
				}
			}
		}
	}

}

void LiftingManager::checkForLiftOff(Building b){
	//check if wall needs to be lifted, and lift the building

	//double check wall is part of wall
	if (b.isPartOfWall && !b.buildingUnit->isLifted()){
		//checks if building is a Barracks or a Factory, only two buildings in wall we are potentially working with
		if (b.buildingUnit->getType() == BWAPI::UnitTypes::Terran_Barracks 
			|| b.buildingUnit->getType() == BWAPI::UnitTypes::Terran_Factory){
			
			if (enemyUnitsNear(b)){
				return;
			}
			//check if one of our units is attempting to go through the wall
			if (unitsTryingToGoThroughWall(b)){
				UnitCommand::lift(b.buildingUnit);
			}
		}
	}
}

void LiftingManager::checkForSetDown(Building b){
	//set down a wall building

	//double checks that wall is lifted
	//if enemies are nearby, or no units are attempting to get through wall
	if (b.buildingUnit->isLifted() && !enemyUnitsNear(b) && unitsTryingToGoThroughWall(b)){
		UnitCommand::land(b.buildingUnit, b.buildingUnit->getInitialTilePosition());
	}
}

bool LiftingManager::enemyUnitsNear(Building b){
	if (b.buildingUnit->getUnitsInRadius(400, Filter::IsEnemy).size() > 0){
		return true;
	}
	return false;
}

bool LiftingManager::myUnitsNear(Building b){
	if (b.buildingUnit->getUnitsInRadius(50, Filter::IsOwned).size() > 0){
		return true;
	}
	return false;
}

bool LiftingManager::unitsTryingToGoThroughWall(Building b){
	UnitData myUnitData = InformationManager::Instance().getUnitData(BWAPI::Broodwar->self());
	for (auto myUnit : myUnitData.getUnits()){
		if (!myUnit.second.type.isBuilding() && myUnit.first->isMoving() && myUnitsNear(b)){
			return true;
		}
	}
	return false;
}