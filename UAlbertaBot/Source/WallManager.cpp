#include "WallManager.h"

using namespace UAlbertaBot;

WallManager::WallManager(){}

WallManager::WallManager(BWAPI::TilePosition defensePoint)//, BWAPI::Region close, BWAPI::Region farSide)
{
    count = 0;
    // We just started, we can't have found a wall yet
    foundWall = false;
    // This might be vistigial code
    //closeRegion = close;
    //farRegion = farSide;

    y = x = yGoal = xGoal = 0;
    // Generate a bounding box around the point we want to defend
    box = buildBoundingBox(defensePoint);
    buildings[0] = 2;
    buildings[1] = 3;
    buildings[2] = 3;
    // Dimensions of a Barracks
    buildingSize[0][0] = 4; // Width
    buildingSize[0][1] = 3; // Height

    // Dimensions of a SupplyDepot
    buildingSize[1][0] = 3; // Width
    buildingSize[1][1] = 2; // Height

    // Dimensions of a SupplyDepot
    buildingSize[2][0] = 3; // Width
    buildingSize[2][1] = 2; // Height
}

BoundingBox WallManager::buildBoundingBox(BWAPI::TilePosition chokePoint){
    BoundingBox box;
    // a 8 by 8 tile array including choke tile
    int startTileX = chokePoint.x - 7;
    int startTileY = chokePoint.y - 7;
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

    int newX;
    int newY;

    for (size_t i = 0; i < box.map.size(); ++i){

        for (size_t j = 0; j < box.map.size(); ++j){
            box.map[j][i] = BWAPI::Broodwar->isBuildable(i + startTileX, j + startTileY);
        }
    }

    for (size_t i = 0; i < walkable.size(); ++i){
        
        for (size_t j = 0; j < walkable.size(); ++j){
            
            newX = (i + startTileX-1);
            newY = (j + startTileY-1);

            bool canWalk = BWTA::getGroundDistance(BWAPI::TilePosition(newX, newY), BWAPI::TilePosition(newX,  newY + 1)) > -1;
            // Let's see if for every buildable tile we can walk to it
            // the problem with this is it's not high enough resolution
            /*bool top = BWTA::isConnected(newX, newY, newX, newY - 1);
            bool bottom = BWTA::isConnected(newX, newY, newX, newY + 1);
            bool left = BWTA::isConnected(newX, newY, newX - 1, newY);
            bool right = BWTA::isConnected(newX, newY, newX+1, newY);*/
           /* int current = BWAPI::Broodwar->getGroundHeight(newX, newY);
            bool top = BWAPI::Broodwar->getGroundHeight(newX, newY - 1) - current < 3;
            bool bottom = BWAPI::Broodwar->getGroundHeight(newX, newY + 1) - current < 3;
            bool left = BWAPI::Broodwar->getGroundHeight(newX - 1, newY) - current < 3;
            bool right = BWAPI::Broodwar->getGroundHeight(newX + 1, newY) - current < 3;*/
            //* top * bottom * left * right
            /*bool topLeft = BWAPI::Broodwar->isWalkable(newX * 4, newY * 4);
            bool topRight = BWAPI::Broodwar->isWalkable(newX * 4 +1, newY * 4);
            bool bottomLeft = BWAPI::Broodwar->isWalkable(newX * 4, newY * 4 +1);
            bool bottomRight = BWAPI::Broodwar->isWalkable(newX * 4 +1, newY * 4 +1);*/
            //(topLeft * topLeft * bottomRight * bottomLeft )
            walkable[j][i] = canWalk;
        }
    }
    return box;
}

void WallManager::findWall(int depth){
    //Debug file
    std::ofstream debug;
    debug.open("debug.txt", std::ios_base::app);

    // Problem, need to find close and far regions

    // If we've found the wall we can stop the search
    if (foundWall){
        return;
     }
    // If we've placed all the buildings, do they meet our requirements
    if (depth == buildings.size()){
        // Print out the state of the wall manager
        // Is it walled off?
        bool canWalk = floodFillInit(0, 0);
        count++;

        if (!canWalk){
            debug << *this;

            //lift barracks somehow, can we still pass through?
            bool canWalk = floodFillInit(0, 0, 2);

            if (canWalk){
                
                // If it's not blocked off without baracks we're good to go.
                foundWall = true;
                debug << *this;
                BWAPI::Broodwar->printf("Size of buildingPos %d", buildingPos.size());
                Barracks = buildingPos[0];
                SupplyDepot1 = buildingPos[1];
                SupplyDepot2 = buildingPos[2];
            }
        }
        return;
    } else {
        int building = buildings[depth];
        for (int x = 0; x < int(box.map.size()) ; ++x){
            for (int y = 0; y < int(box.map.size()); ++y){
                // See if we can place the building down in such a way as to build part of a wall
                if (properWall(x, y, building, depth)){
                    // Define tile locations of buildings
                    mapOutPlacement(x, y, depth, building);
                    // This is a good place to put the next building
                    buildingPos.push_back(BWAPI::TilePosition(x + box.start.x, y + box.start.y));
                    findWall(depth + 1);
                    // Maybe not so good of a place
                    buildingPos.pop_back();
                    // Let's unmap it's layout
                    // 0 for blocked, 1 for free, >1 for type of building
                    mapOutPlacement(x, y, depth, 1);
                }
            }
        }

    }
}
// Most likely an A* algorithm, possibly floodfill
// bool WallManager::WalledOff(int buildingType){
//     return false;
// }

bool WallManager::properWall(int x, int y, int buildingNumber, int depth){
    bool neighbour = false;
    Building building;
    BWAPI::TilePosition currentTile = BWAPI::TilePosition(x + box.start.x, y + box.start.y);
    //Don't know how to get that information
    if (depth == 0){
        UnitType unit = MetaType(BWAPI::UnitTypes::Terran_Barracks).getUnitType();
        building = Building(unit, currentTile);
    }
    else{
        UnitType unit = MetaType(BWAPI::UnitTypes::Terran_Supply_Depot).getUnitType();
        building = Building(unit, currentTile);
    }
    if (!BuildingPlacer::Instance().canBuildHere(currentTile, building)){
        return neighbour;
    }
    if (box.map[y][x] != 1){
        return neighbour;
    }

    int width = buildingSize[depth][0];
    int height = buildingSize[depth][1];
    if (x + width > 16 || y + width > 16){
        return neighbour;
    }
    // We are building Barracks first, so if it's buildable we're good to go
    // Don't have a solid access for this
    if (buildingNumber == 2){
        neighbour = true;
    }
    // If it's not a Barracks, it should be beside another unit
    // x and y being all pairwise combinations of x and y;
    int dx[] = {1, 1, 1, 0, 0, -1, -1,-1};
    int dy[] = { 1, -1, 0, 1, -1, 1, -1, 0 };
    
    
    //For all spots around x and y, is there a building
    for (int i = x; i < x + width ; ++i){
        for (int j = y; j < y+height; ++j){
            for (int k = 0; k < 8; ++k){

                int deltaX = i + dx[k];
                int deltaY = j + dy[k];
                //check to see if there are anybuildings near outer tiles
                // as well, if they are make sure the gap isn't too big
                
                // If they are are the edge, continue
                if (deltaX < 0 || deltaY < 0){
                    continue;
                }
                if (deltaX >= box.map.size() || deltaY >= box.map.size()){
                    continue;
                }
                // Check to see if the building will be in a resource zone
                BWAPI::Unitset set = BWAPI::Broodwar->getUnitsOnTile(box.start.x + i, box.start.y + j);
                for (auto unit : set){
                    if (unit->getType() == BWAPI::UnitTypes::Resource_Vespene_Geyser){
                        return false;
                    }
                }
                // Prevent overlapping buildings
                if (box.map[j][i] != 1){
                    return false;
                }
                if (box.map[deltaY][deltaX] >= 2){
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

void WallManager::mapOutPlacement(int x, int y, int buildingType, int fillNumber){
    int width = buildingSize[buildingType][0];
    int height = buildingSize[buildingType][1];
    assert(y + height < box.map.size());
    assert(x + width < box.map.size());
    for(int i=x; i< x + width ; ++i){
        for(int j=y; j < y + height ; ++j){
            box.map[j][i] = fillNumber;
        }
    }
}

// Adapt these two helper function to implement floodfill
bool WallManager::floodFillInit(int x, int y, int barracks){
    int width = BWAPI::Broodwar->mapWidth();
    int height = BWAPI::Broodwar->mapHeight();
    
    //Debug stuff
    //walked.empty();
    for (size_t i = 0; i < walked.size(); ++i){
        for (size_t j = 0; j < walked.size(); ++j){
            //walked[j][i] = BWTA::getGroundDistance(BWAPI::TilePosition(box.start.x + i ,box.start.y + j), BWAPI::TilePosition(box.start.x + i, box.start.y +j+1));
                //walked[j][i] = BWAPI::Broodwar->getRegionAt((box.start.x + i)*32, (box.start.y+j)*32)->getID();
            walked[j][i] = 0;
        }
    }
    // Generate key for map
    if (x < 0 || x >= width){
        // Bounds check for x
        return false;
    }
    else if (y < 0 || y >= height){
        // Bounds check for y
        return false;
    } else {
    
        int yGoal;
        for (size_t i = x; i < walkable.size(); ++i){
            yGoal = findGoodYPos(i, y, 1);
            if (yGoal>-1){
                x = i;
                break;
            }
        }
        y = yGoal;
        int xGoal = 17;
        for (size_t i = 17; i > 0; --i){
            yGoal = findGoodYPos(i, 17, -1);
            if (yGoal>-1){
                xGoal = i;
                break;
            }
        }
        this->y = y;
        this->x = x;
        this->yGoal = yGoal;
        this->xGoal = xGoal;
        if (yGoal == -1){
            return false;
        }
        int tileType = 1;
        return floodFill(x, y, tileType, xGoal, yGoal, barracks);
    }
}

bool WallManager::floodFill(const int x, const int y, int tileNumber, int xGoal, int yGoal, int barracks) const{
    int width = BWAPI::Broodwar->mapWidth();
    int height = BWAPI::Broodwar->mapHeight();
    if (x < 0 || x >= walkable.size()){
        // Bounds check for x
        return false;
    } else if (y < 0 || y >= walkable.size()){
        // Bounds check for y
        return false;
    /*} else if (!canFit(size, x, y)){
        return false;*/
    //} else if(x < box.start.x){
    //    // reached min x
    //    return false; 
    //} else if(y < box.start.y){
    //    // reached min y
    //    return false; 
    } else if(y >= walkable.size()){
        return false;
    } else if(x >= walkable.size()){
        return false;
    } 
    walked[y][x] = 0;
    if (walkable[y][x] != 1){
        return false;
    }
    if (y > 0 && y< 17 && x > 0 && x < 17){
        if (box.map[y - 1][x - 1] > barracks){
            return false;
        }
    }
    walked[y][x] = 1;
    if(x == xGoal && y == yGoal){
        return true;
    } else{
        // North
       /* if (floodFill(x, y - 1, tileNumber, xGoal, yGoal, barracks)){
            return true;
        }*/
        if (floodFill(x + 1, y, tileNumber, xGoal, yGoal, barracks)){
            return true;
        }
        // South 
        if (floodFill(x, y+1, tileNumber, xGoal, yGoal, barracks)){
            return true;
        }
        //East
        
        // West
        // floodFill(size, x - 1, y, group, tile);
    }
    return false;
}

int WallManager::findGoodYPos(int x, int y, int travelDirection) const{
    int max = walkable.size();
    int yPos;
    for (int i = 0; i < max; ++i){
        yPos = y + travelDirection*i;
        if (yPos > 0 && yPos< 17 && x > 0 && x < 17){
            if (box.map[yPos - 1][x - 1] != 1){
                continue;
            }
        }
        if (walkable[yPos][x] == 1){
            return yPos;
        }
    }
    return -1;
}

// For debugging the wall manager
std::ostream& operator<<(std::ostream & out, const WallManager & wallmanager){
    static int iteration = 0;
    out << "WallManager state: " << std::endl;
    out << "Iteration : " << iteration++ << std::endl;
    out << "Found Wall? :" << wallmanager.foundWall << std::endl;

    out << "Building Sizes: " << std::endl;
    int buildingCount = 0;
    for (const auto building : wallmanager.buildingSize){
        out << "Building: " << buildingCount++ << " ";
        out << building[0] << " by " << building[1] << std::endl;
    }
    out << "x: " << wallmanager.x << " y: " << wallmanager.y << " xGoal: " << wallmanager.xGoal << " yGoal: " << wallmanager.yGoal << std::endl;
    std::stringstream buildings;
    std::stringstream walkable;
    std::stringstream walked;
    out << "Bounding Box: " << std::endl;
    BoundingBox box = wallmanager.box;
    for (size_t i = 0; i < box.map.size(); ++i){
        for (size_t j = 0; j < box.map.size(); ++j){
            buildings << box.map[i][j] << " ";
            
        }
        out << buildings.rdbuf() <<  std::endl;
        buildings.clear();
        
    }

    out << "Is Walkable" << std::endl;
    for (size_t i = 0; i < wallmanager.walkable.size(); ++i){
        for (size_t j = 0; j < wallmanager.walkable[i].size(); ++j){
            walkable << wallmanager.walkable[i][j] << " ";
            walked << wallmanager.walked[i][j] << " ";
        }
        out << walkable.rdbuf() << "\t" << walked.rdbuf() << std::endl;
        walkable.clear();
        walked.clear();
    }
    /*
    out << "Walked" << std::endl;
    for (size_t i = 0; i < wallmanager.walked.size(); ++i){
        for (size_t j = 0; j < wallmanager.walked[i].size(); ++j){
            out << wallmanager.walked[i][j] << " ";
        }
        out << std::endl;
    }*/
    out << "------------" << std::endl << std::endl;
    return out;
}