#include "LiftingManager.h"

using namespace UAlbertaBot;

LiftingManager & LiftingManager::Instance()
{
	static LiftingManager instance;
	return instance;
}

void LiftingManager::checkForLiftOff(){
	//check if wall needs to be lifted, and lift the building


}

void LiftingManager::checkForSetDown(){
	//set down a wall building


}