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
		void	update();

	private:
		bool	_inTheProcessOfLanding = false;

		void	checkForLiftOff(Building b);
		void	checkForSetDown(Building b);
		bool	enemyUnitsNear(Building b);
		bool	myUnitsNear(Building b);
	};
}