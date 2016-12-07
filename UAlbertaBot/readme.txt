Compile instructions:
import project into Visual Studio 2013 and build


Changes:

GhostManager.h
GhostManager.cpp, executeMicro
GhostManager.cpp, getTarget
GhostManager.cpp, getAttackPriority

TankManager.cpp, executeMicro

MicroManager.cpp, execute

Squad.h
Squad.cpp, update
Squad.cpp, addUnitsToMicroManagers

SquadOrder.h, SquadOrderTypes

CombatCommander.h
CombatCommander.cpp, initializeSquads
CombatCommander.cpp, update
CombatCommander.cpp, updateTankDefenseSquad
CombatCommander.cpp, (added priority for tank defense squad)

StrategyManager.cpp, getTerranBuildOrderGoal

Config.cpp (added UseWallingAsTerran bool)

BuildingManager.cpp, assignWorkersToUnassignedBuildings
BuildingManager.cpp, checkForCompletedBuildings
BuildingManager.h, added a parameter to addBuildingTask
BuildingManager.h, added a buildingsWeHaveBuilt vector to track all buildings we have built

GameCommander.cpp, update
	(updating the lifting manager)

InformationManager.cpp, drawMapInformation
	wrote some debug code for walling

LiftingManager.cpp
LiftingManager.h

MetaType.cpp, constructor
MetaType.cpp, isPartOfWall
MetaType.h, added _partOfWall bool

ProductionManager.cpp, constructor
ProductionManager.cpp, update
ProductionManager.cpp, manageBuildOrderQueue
ProductionManager.cpp, create
ProductionManager.cpp, getMadeFirstWall
ProductionManager.cpp, getCurrentlyBuildingWall
ProductionManager.h, added some class attributes

BuildingData.h (added isPartOfWall bool to a building object, built another constructor for it)
