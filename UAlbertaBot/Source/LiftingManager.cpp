#include "LiftingManager.h"

using namespace UAlbertaBot;

LiftingManager & LiftingManager::Instance()
{
	static LiftingManager instance;
	return instance;
}

void LiftingManager::checkForLiftOff(Building b){
	//check if wall needs to be lifted, and lift the building

	//double check wall is part of wall
	if (b.isPartOfWall && !b.buildingUnit->isLifted()){
		//checks if building is a Barracks or a Factory, only two buildings in wall we are potentially working with
		if (b.buildingUnit->getType() == BWAPI::UnitTypes::Terran_Barracks 
			|| b.buildingUnit->getType() == BWAPI::UnitTypes::Terran_Factory){
			
			//check if one of our units is attempting to go through the wall, but can't

			//LIFT
			//UnitCommand::lift(b.buildingUnit);
		}

	}
}

void LiftingManager::checkForSetDown(Building b){
	//set down a wall building

	//double checks that wall is lifted
	//if enemies are nearby, or no units are attempting to get through wall
	if (b.buildingUnit->isLifted()){




		//UnitCommand::land(b.buildingUnit, b.buildingUnit->getInitialTilePosition());
	}
}