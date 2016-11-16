#pragma once

#include "Common.h"

namespace UAlbertaBot
{

class WallManager {
public: 
    WallManager();
    findWall();
    bool WalledOff();
    bool properWallI();
    bool maxGap();
    getSupplyDepot1();
    getBarracks()
    getSupplyDepot2();

private:
    bool foundWall;

}
}