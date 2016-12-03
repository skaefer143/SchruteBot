#pragma once

#include "Common.h"
#include "MetaType.h"
#include "BuildingManager.h"
#include <fstream>
#include <BWAPI.h>

namespace UAlbertaBot
{
	class LiftingManager{
	public:
		static LiftingManager &	Instance();
		void	checkForLiftOff(Building b);
		void	checkForSetDown(Building b);


	private:

		bool	enemyUnitsNear(Building b);
		bool	myUnitsNear(Building b);
		bool	unitsTryingToGoThroughWall(Building b);
	};
}