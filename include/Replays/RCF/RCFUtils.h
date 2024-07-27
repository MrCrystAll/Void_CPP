#pragma once

#include <Replays/ReplayUtils.h>
#include <RLGymSim_CPP/Utils/Gamestates/GameState.h>

using namespace RLGSC;

#define RCF_NS RCF
#define START_RCF_NS START_REPLAY_SUB_NS(RCF_NS)
#define END_RCF_NS END_REPLAY_NS
#define USE_RCF_NS USE_REPLAY_SUB_NS(RCF_NS)

START_RCF_NS

bool isGoalScored(const GameState state);
bool IsOnWall(const PlayerData player, int wallHeightTolerance = 100, int wallWidthTolerance = 100);
bool IsOnCeiling(const PlayerData player, int wallHeightTolerance = 100, int wallWidthTolerance = 100);

END_RCF_NS