#pragma once

#define VOID_REPLAYS_TAG "[Void_Replays]"

#include <RLGymSim_CPP/Utils/Gamestates/GameState.h>

class StateGenerator {
public:
	static RLGSC::GameState CreateGroundState(int nBluePlayers, int nOrangePlayers);
	static RLGSC::GameState CreateAirState(int nBluePlayers, int nOrangePlayers);
	static RLGSC::GameState CreateOnWallState(int nBluePlayers, int nOrangePlayers);
	static RLGSC::GameState CreateOnCeilingState(int nBluePlayers, int nOrangePlayers);
	static RLGSC::GameState CreateOnCornerState(int nBluePlayers, int nOrangePlayers);
private:
	static RocketSim::Arena* CreateArena(int nBluePlayers, int nOrangePlayers);
};

RLGSC::GameState groundState = StateGenerator::CreateGroundState(1, 1);
RLGSC::GameState airState = StateGenerator::CreateAirState(1, 1);
RLGSC::GameState wallState = StateGenerator::CreateOnWallState(1, 1);
RLGSC::GameState ceilingState = StateGenerator::CreateOnCeilingState(1, 1);
RLGSC::GameState cornerState = StateGenerator::CreateOnCornerState(1, 1);
