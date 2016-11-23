#include "WallManager.h"


using namespace UAlbertaBot;

WallManager::WallManager(BWAPI::TilePosition defensePoint, BWAPI::Region close, BWAPI::Region far){
	
    // We just started, we can't have found a wall yet
    foundWall = false;
    // This might be vistigial code
    closeRegion = close;
    farRegion = far;

    // Generate a bounding box around the point we want to defend
    box = buildBoundingBox(defensePoint);
    buildings = {3, 2, 2};
    // Dimensions of a Barracks
    buildingSize[0][0] = 3; // Width
    buildingSize[0][1] = 2; // Height

    // Dimensions of a SupplyDepot
    buildingSize[1][0] = 2; // Width
    buildingSize[1][1] = 2; // Height
}

BoundingBox WallManager::buildBoundingBox(BWAPI::TilePosition chokePoint){
    BoundingBox box;
    // a 8 by 8 tile array including choke tile
    int startTileX = chokePoint.x - 8;
    int startTileY = chokePoint.y - 8;
    int endTileX = chokePoint.x + 8;
    int endTileY = chokePoint.y + 8;

    if (startTileX < 0){
        startTileX = 0;
    }

    if (startTileY < 0){
        startTileY = 0;
    }

    int width = BWAPI::Broodwar->mapWidth();
    int height = BWAPI::Broodwar->mapHeight();
    if (endTileX >= width){
        endTileX = width - 1;
    }

    if (endTileY >= height){
        endTileY = height - 1;
    }

    box.start = BWAPI::TilePosition(startTileX, startTileY);
    box.end = BWAPI::TilePosition(endTileX, endTileY);
    for (int i = 0; i < 17; ++i){
        for (int j = 0; j < 17; ++j){
            box.map[i][j] = BWAPI::Broodwar->isBuildable(i, j);
        }
    }
    return box;
}

void WallManager::findWall(int depth){
    // Problem, need to find close and far regions

    // If we've found the wall we can stop the search
    if (foundWall){
        return;
     }

    // If we've placed all the buildings, do they meet our requirements
    if (depth == buildings.size()){
        // Is it walled off?
        bool walkable = floodFillInit(box.start.x, box.start.y);
        if (!walkable){
            //lift barracks somehow, can we still pass through?
            bool walkable = floodFillInit(box.start.x, box.start.y, 3);
            if (walkable){
                // If it's not blocked off without baracks we're good to go.
                foundWall = true;
                Barracks = buildingPos[0];
                SupplyDepot1 = buildingPos[1];
                SupplyDepot2 = buildingPos[2];
            }
        }
        return;
    } else {
        int building = buildings[depth];
        for (int x = box.start.x; x < box.end.x; ++x){
            for (int y = box.start.y; y < box.end.y; ++y){
                // See if we can place the building down in such a way as to build part of a wall
                if (properWall(x, y, building, buildingType)){
                    // Define tile locations of buildings
                    mapWallPlacement(x, y, BuildingType, buildingType);
                    // This is a good place to put the next building
                    buildingPos.push_back(BWAPI::TilePosition(x, y));
                    findWall(Buildings, depth + 1);

                    // Maybe not so good of a place
                    buildingPos.pop_back();
                    // Let's unmap it's layout
                    // 0 for blocked, 1 for free, >1 for type of building
                    mapWallPlacement(x, y, BuildingType, 1);
                }
            }
        }

    }
}
// Most likely an A* algorithm, possibly floodfill
// bool WallManager::WalledOff(int buildingType){
//     return false;
// }

bool WallManager::properWall(int x, int y, int buildingNumber){
    bool neighbour = false;
    //Don't know how to get that information
    if (!BuildingPlacer::Instance().buildable(building, x, y)){
        return neighbour;
    }
    // We are building Barracks first, so if it's buildable we're good to go
    // Don't have a solid access for this
    if (buildNumber == 3){
        return true;
    }
    // If it's not a Barracks, it should be beside another unit
    // x and y being all pairwise combinations of x and y;
    int dx[] = {1, 1, 1, 0, 0, -1, -1,-1};
    int dy[] = { 1, -1, 0, 1, -1, 1, -1, 0 };
    
    //For all spots around x and y, is there a building
    for (int i = x; x < x + buildingSize[buildingNumber][0] ; ++i){
        for (int j = y; y < y+buildingSize[buildingNumber][1]; ++j){
            for (int k = 0; k < 8; ++k){
                
                int deltaX = i + dx[k];
                int deltaY = j + dy[k];
                //check to see if there are anybuildings near outer tiles
                // as well if there are make sure the gap isn't too big
                if(box.map[deltaX][deltaY] => 2){
                    // if there is a neighbour make sure it doesn't violate the max gap principle
                    // neighbour = maxGap()
                    // Eventually need to check for maxgap but for now we're happy that it has a nei
                    neighbour = true;
                }
                // Idea for how to use maxgap 
                // if(neighbour){
                //     if(maxGap()){
                //         neighbour = false;
                //     } else {
                //         return neighbour;
                //     }
                // }
            }
        }
    }

    return neighbour;
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

void WallManager::mapOutPlacement(int x, int y, int BuildingType, int fillNumber){
    for(int i=x; i< x + buildingSize[buildingType][0]; ++i){
        for(int j=y; k < y + buildingSize[buildingType][1]; ++i){
            box.map[i][j] = fillNumber;
        }
    }
}

// Adapt these two helper function to implement floodfill
void WallManager::floodFillInit(int x, int y, int barracks=1) const{
    // Generate key for map
    if (x < 0 || x >= this->width){
        // Bounds check for x
        return;
    }
    else if (y < 0 || y >= this->height){
        // Bounds check for y
        return;
    } else {

        findGoalPos(x, y);
        int tileType = 1;
        return floodFill(x, y, tileType, xGoal, yGoal, barracks);
    }
}

void Grid::floodFill(const int x, const int y, int tileNumber, int xGoal, int yGoal, int barracks) const{
    if (x < 0 || x >= this->_width){
        // Bounds check for x
        return false;
    } else if (y < 0 || y >= this->_height){
        // Bounds check for y
        return false;
    } else if (!canFit(size, x, y)){
        return false;
    } else if(x < box.start.x){
        // reached min x
        return false; 
    } else if(y < box.start.y){
        // reached min y
        return false; 
    } else if(y > box.end.y){
        return false;
    } else if(x > box.end.x){
        return false;
    }



    else if (box.map[x][y] != tileNumber && box.map[x][y] != barracks){
        return false;
    }
    if(x == xGoal && y == yGoal){
        return true;
    } else{
        // North
        //floodFill(size, x, y - 1, group, tile);
        
        // South 
        if(floodFill(size, x, y + 1, group, tile)){
            return true;
        }
        //East
        if(floodFill(size, x + 1, y, group, tile)){
            return true;
        }
        // West
        // floodFill(size, x - 1, y, group, tile);
    }
    return false;
}
