#include "GhostManager.h"
#include "UnitUtil.h"

using namespace UAlbertaBot;

GhostManager::GhostManager() 
{ 
	
}

void GhostManager::executeMicro(const BWAPI::Unitset & targets)
{
	const BWAPI::Unitset & ghosts = getUnits();

	// figure out targets
	BWAPI::Unitset ghostTargets;
	std::copy_if(targets.begin(), targets.end(), std::inserter(ghostTargets, ghostTargets.end()),
		[](BWAPI::Unit u){ return u->isVisible();  });

	//set the range of the ghost
	int ghostRange = BWAPI::UnitTypes::Terran_Ghost.groundWeapon().maxRange() - 16;
	//set energy requirements
	int cloakingEnergy = BWAPI::UnitTypes::Terran_Ghost.cloakingTech().energyCost;
	int lockdownEnergy = BWAPI::TechTypes::Lockdown.energyCost();


	for (auto & ghost : ghosts)
	{
		bool ghostNearEnemy = false;
		if (order.getType() == SquadOrderTypes::Attack)
		{
			// if attacking, do not attack units

			// find the best target for this tank
			BWAPI::Unit target = getTarget(ghost, ghostTargets);
			if (!targets.empty()){

				
				// cloak
				if (ghost->getDistance(target) < 2 * ghostRange && !ghost->isCloaked && ghost->canCloak() && ghost->getEnergy > 1.5*cloakingEnergy){
					ghostNearEnemy = true;
					ghost->cloak();
				}

				//lockdown enemy carriers
				if (target->getType == BWAPI::UnitTypes::Protoss_Carrier && !target->isLockedDown() && ghost->getEnergy > lockdownEnergy){
					//maybe check if the tech has been researched
					ghost->useTech(BWAPI::TechTypes::Lockdown, target);
				}

			}
		}
		else if (order.getType() == SquadOrderTypes::Defend)
		{
			// find the best target for this tank
			BWAPI::Unit target = getTarget(ghost, ghostTargets);
		}
	}
}





/*

nuking implentation:
-wait until nuke is ready
-move to enemy base
-cloke if enemy is in range
-find building target
-launch nuke

anti-carrier implementation
-find carrier target
-lockdown
-shoot carrier
-refresh lockdown if it's not dead. 

*/