#pragma once;

#include <Common.h>
#include "MicroManager.h"

namespace UAlbertaBot
{
	class VultureManager : public MicroManager
	{
	public:

		VultureManager();
		void executeMicro(const BWAPI::Unitset & targets);

		BWAPI::Unit chooseTarget(BWAPI::Unit rangedUnit, const BWAPI::Unitset & targets, std::map<BWAPI::Unit, int> & numTargeting);
		BWAPI::Unit closestrangedUnit(BWAPI::Unit target, std::set<BWAPI::Unit> & rangedUnitsToAssign);

		int getAttackPriority(BWAPI::Unit rangedUnit, BWAPI::Unit target);
		BWAPI::Unit getTarget(BWAPI::Unit rangedUnit, const BWAPI::Unitset & targets);

		//time to override a non virtual function
		void regroup(const BWAPI::Position & regroupPosition) const;
		void vultureDanceTarget(BWAPI::Unit vulture, BWAPI::Unit target);
	};
}