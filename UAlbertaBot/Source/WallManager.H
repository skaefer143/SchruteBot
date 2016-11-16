#pragma once

#include "Common.h"
#include <BWAPI.h>

// Algorithm borrowed from
// http://satirist.org/ai/starcraft/blog/archives/151-LetaBots-wall-4-placing-buildings-to-form-the-wall.html on Nov 14 
// http://www.aaai.org/ocs/index.php/AIIDE/AIIDE14/paper/viewFile/8956/8927 looked at implementing
// https://arxiv.org/pdf/1306.4460.pdf
using namespace BWAPI;

class WallManager {
public: 
    WallManager();
    void findWall(std::vector<BWAPI::Unit> Buildings, int depth));
    bool WalledOff();
    bool properWall();
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
};