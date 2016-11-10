#include "GhostManager.h"
#include "UnitUtil.h"

using namespace UAlbertaBot;

GhostManager::GhostManager() 
{ 
}

/*

gameplan: use informationManager to find the base location. once the base location is known, move the ghost to that location
keep ghost cloaked while moving through base. Once at base, hide behind resources. Once nuke is ready, cloak, find command centre/nexus/hatchery
 (refered to as Nexus for simplicity) and send a nuke near it. once nuke is on the way, move the ghost away. 

 if sneaking through base is imposiible, then use ghost to nuke defenses

*/

/*

implentation:
-find base location
-move to enemy base
-find Nexus
-know if there is a nuke available
-sneak through base
-avoid detectors

*/