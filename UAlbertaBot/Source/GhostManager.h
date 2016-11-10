#pragma once;

#include <Common.h>
#include "MicroManager.h"

namespace UAlbertaBot
{
class GhostManager : public MicroManager
{
public:

	GhostManager();
	void executeMicro(const BWAPI::Unitset & targets);

	int getAttackPriority(BWAPI::Unit rangedUnit, BWAPI::Unit target);
	BWAPI::Unit getTarget(BWAPI::Unit rangedUnit, const BWAPI::Unitset & targets);

	bool shouldNuke();
};
}