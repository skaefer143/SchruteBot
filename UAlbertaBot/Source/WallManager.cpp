#include "WallManager.h"


using namespace UAlbertaBot;
WallManager::WallManager(BWAPI::Region close, BWAPI::Region far){
	foundWall = false;
    closeRegion = close;
    farRegion = far;
}
	
void WallManager::findWall(std::vector<BWAPI::Unit> Buildings, int depth){
    // Problem, need to find close and far regions


    // If we've found the wall we can stop the search
    if (foundWall){
        return;
	}

    // If we've placed all the buildings, do they meet our requirements
	if (depth == Buildings.size()){
        // Is it walled off?
        bool blockedOff = WalledOff();
        if (blockedOff){

            //lift barracks somehow, can we still pass through?
            bool blockedOffWithoutBarracks = WalledOff();
            if (!blockedOffWithoutBarracks){
                // If it's not blocked off without baracks we're good to go.
                foundWall = true;
                Barracks = buildingPos[0];
                SupplyDepot1 = buildingPos[1];
                SupplyDepot2 = buildingPos[2];
            }
        }
        return;
    } else {

    }
}


bool WallManager::WalledOff(){
    return false;
}
bool WallManager::properWall(){
    return false;
}
bool WallManager::maxGap(){
    return false;
}
BWAPI::TilePosition  WallManager::getSupplyDepot1(){
    return SupplyDepot1;
}
BWAPI::TilePosition  WallManager::getBarracks(){
    return Barracks;
}
BWAPI::TilePosition	 WallManager::getSupplyDepot2(){
    return SupplyDepot2;
}