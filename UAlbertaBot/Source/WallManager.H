#pragma once

#include "Common.h"
#include <BWAPI.h>
#include "BuildingPlacer.h"

// Algorithm borrowed from
// http://satirist.org/ai/starcraft/blog/archives/151-LetaBots-wall-4-placing-buildings-to-form-the-wall.html on Nov 14 
// http://www.aaai.org/ocs/index.php/AIIDE/AIIDE14/paper/viewFile/8956/8927 looked at implementing
// https://arxiv.org/pdf/1306.4460.pdf
using namespace BWAPI;


// Boundingbox for wall region that is of size 10 by 10
struct BoundingBox{
    TilePosition start;
    TilePosition end;
    int map[16][16];
};

class WallManager {
public: 
    WallManager(BWAPI::TilePosition defensePoint, BWAPI::Region close, BWAPI::Region far);
    BoundingBox buildBoundingBox(BWAPI::TilePosition chokePoint);
    void findWall(std::vector<BWAPI::Unit> Buildings, int depth);
    bool WalledOff(int buildingType);
    bool properWall(int x, int y, const Building &Building, int buildingNumber);
    void mapOutPlacement(int x, int y, int BuildingType, int BuildingType2);
    bool maxGap();
    BWAPI::TilePosition getSupplyDepot1();
    BWAPI::TilePosition getBarracks();
    BWAPI::TilePosition	getSupplyDepot2();

private:
    bool foundWall;
    BWAPI::TilePosition SupplyDepot1;
    BWAPI::TilePosition Barracks;
    BWAPI::TilePosition SupplyDepot2;

    std::vector<BWAPI::TilePosition> buildingPos;
    BWAPI::Region closeRegion;
    BWAPI::Region farRegion;
    int buildingGaps[2][4];
    int buildingSize[2][2];
    BoundingBox box;   
};
