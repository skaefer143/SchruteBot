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
};
}