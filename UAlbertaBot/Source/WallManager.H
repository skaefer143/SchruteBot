#pragma once

#include "Common.h"
#include "MetaType.h"
#include "BuildingManager.h"
#include <fstream>
#include <BWAPI.h>

// Algorithm borrowed from
// http://satirist.org/ai/starcraft/blog/archives/151-LetaBots-wall-4-placing-buildings-to-form-the-wall.html on Nov 14 
// http://www.aaai.org/ocs/index.php/AIIDE/AIIDE14/paper/viewFile/8956/8927 looked at implementing
// https://arxiv.org/pdf/1306.4460.pdf
using namespace BWAPI;


// Boundingbox for wall region that is of size 10 by 10
struct BoundingBox{
    TilePosition start;
    TilePosition end;
    std::array<std::array<int, 16>, 16> map;
};

class WallManager {
public: 
	static	WallManager &	    Instance();

	WallManager();
    WallManager(BWAPI::TilePosition defensePoint); //BWAPI::Region close, BWAPI::Region far);
    BoundingBox buildBoundingBox(BWAPI::TilePosition chokePoint);
    void findWall(int depth);
    bool WalledOff(int buildingType);
    bool properWall(int x, int y, int buildingNumber, int depth);
    void mapOutPlacement(int x, int y, int BuildingType, int BuildingType2);
    bool maxGap();
    bool goodWall(){ return foundWall; };
    BWAPI::TilePosition getSupplyDepot1();
    BWAPI::TilePosition getBarracks();
    BWAPI::TilePosition	getSupplyDepot2();
	BWAPI::TilePosition	getFactory();
    BoundingBox box;
    friend std::ostream &operator<<(std::ostream & out, const WallManager & wm_in);
private:
    bool floodFillInit(int x, int y, int barracks=1);
    bool floodFill(int x, int y, int tileType, int xGoal, int yGoal, int barracks) const;   
    int findGoodYPos(int x, int y, int travelDirection) const;
    
    bool foundWall;
    BWAPI::TilePosition SupplyDepot1;
    BWAPI::TilePosition Barracks;
    BWAPI::TilePosition SupplyDepot2;
	BWAPI::TilePosition Factory;
    std::vector<BWAPI::TilePosition> buildingPos;
    BWAPI::Region closeRegion;
    BWAPI::Region farRegion;
    std::array<int,3> buildings;
    std::array<std::array<int, 4>,2> buildingGaps;
    std::array<std::array<int, 2>, 3>buildingSize;
    int count;
    mutable int x, y, yGoal, xGoal;
    std::array< std::array<int, 18>, 18 > walkable;
    mutable std::array< std::array<int, 18>, 18 > walked;
   // BoundingBox box;   

	bool WallManager::checkForPreBuiltWall();
};
