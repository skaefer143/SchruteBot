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
