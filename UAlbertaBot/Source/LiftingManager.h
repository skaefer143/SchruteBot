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

	};
}