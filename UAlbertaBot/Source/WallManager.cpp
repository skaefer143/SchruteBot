//#include "WallManager.h"
//
//
//using namespace UAlbertaBot;
//WallManager::WallManager(BWAPI::TilePosition defensePoint, BWAPI::Region close, BWAPI::Region far){
//	foundWall = false;
//    closeRegion = close;
//    farRegion = far;
//
//}
//BoundingBox WallManager::buildBoundingBox(BWAPI::TilePosition chokePoint){
//    BoundingBox box;
//    // a 10 by 10 tile array including choke tile
//    int startTileX = chokePoint.x - 9;
//    int startTileY = chokePoint.y - 9;
//    int endTileX = chokePoint.x + 10;
//    int endTileY = chokePoint.y + 10;
//
//    if (startTileX < 0){
//        startTileX == 0;
//    }
//
//    if (startTileY < 0){
//        startTileY == 0;
//    }
//
//    int width = BWAPI::Broodwar->mapWidth();
//    int height = BWAPI::Broodwar->mapHeight();
//    if (endTileX >= width){
//        endTileX == width - 1;
//    }
//
//    if (endTileY >= height){
//        endTileY = height - 1;
//    }
//
//    box.start = TilePosition(startTileX, startTileY);
//    box.end = TilePosition(endTileX, endTileY);
//    for (int i = 0; i < 10; ++i){
//        for (int j = 0; j < 10; ++j){
//            box.container[i][j] = 1;
//        }
//    }
//
//}
//void WallManager::findWall(std::vector<BWAPI::Unit> Buildings, int depth){
//    // Problem, need to find close and far regions
//
//
//    // If we've found the wall we can stop the search
//    if (foundWall){
//        return;
//	}
//
//    // If we've placed all the buildings, do they meet our requirements
//	if (depth == Buildings.size()){
//        // Is it walled off?
//        bool blockedOff = WalledOff();
//        if (blockedOff){
//
//            //lift barracks somehow, can we still pass through?
//            bool blockedOffWithoutBarracks = WalledOff();
//            if (!blockedOffWithoutBarracks){
//                // If it's not blocked off without baracks we're good to go.
//                foundWall = true;
//                Barracks = buildingPos[0];
//                SupplyDepot1 = buildingPos[1];
//                SupplyDepot2 = buildingPos[2];
//            }
//        }
//        return;
//    } else {
//        BWAPI::Unit building = Buildings[depth];
//        for (int x = box.startX; x < box.endX; ++x){
//            for (int y = box.startY; y < box.endY; ++y){
//                // See if we can place the building down in such a way as to build part of a wall
//                if (properWall(x, y, building)){
//                    // Define tile locations of buildings
//                    mapWall(x, y, BuildingType);
//                    // This is a good place to put the next building
//                    buildingPos.push_back(BWAP::TilePosition(x, y);
//                    RecursiveWall(Buildings, depth + 1);
//
//                    // Maybe not so good of a place
//                    buildingPos.pop_back();
//                    // Let's unmap it's layout
//                    // 0 for blocked, 1 for free, >1 for type of building
//                    mapWall(x, y, BuildingType, 1);
//                }
//            }
//        }
//
//    }
//}
//BWAPI::TilePosition findNearestBuildableTile(){};
//
//bool WallManager::WalledOff(){
//    return false;
//}
//bool WallManager::properWall(int x, int y, int BuildintType){
//    return false;
//}
//bool WallManager::maxGap(){
//    return false;
//}
//BWAPI::TilePosition  WallManager::getSupplyDepot1(){
//    return SupplyDepot1;
//}
//BWAPI::TilePosition  WallManager::getBarracks(){
//    return Barracks;
//}
//BWAPI::TilePosition	 WallManager::getSupplyDepot2(){
//    return SupplyDepot2;
//}
//
//void WallManager::mapOutPlacement(int x, int y, int BuildingType, int BuildingType2)