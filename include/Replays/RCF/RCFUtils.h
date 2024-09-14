/*****************************************************************//**
 * \file   RCFUtils.h
 * \brief  Some utils for the RCF
 * 
 * \author Mathieu Suchet
 * \date   July 2024
 *********************************************************************/

#pragma once

#include <Replays/ReplayUtils.h>
#include <RLGymSim_CPP/Utils/Gamestates/GameState.h>

using namespace RLGSC;

#define RCF_NS RCF
#define START_RCF_NS START_REPLAY_SUB_NS(RCF_NS)
#define END_RCF_NS END_REPLAY_NS
#define USE_RCF_NS USE_REPLAY_SUB_NS(RCF_NS)

START_RCF_NS

/// <summary>
/// Checks if a goal is scored
/// </summary>
/// <param name="state">The state to check</param>
/// <returns>Whether a goal is scored</returns>
bool IsGoalScored(const GameState state);

/// <summary>
/// Checks if a player is on the wall
/// </summary>
/// <param name="player">The player to check</param>
/// <param name="wallHeightTolerance">The distance from the ground and off the ceiling where the player starts to be considered on wall</param>
/// <param name="wallWidthTolerance">The distance off the walls where the players starts to be considered on the wall</param>
/// <returns>Whether the player is on the wall</returns>
bool IsOnWall(const PlayerData player, int wallHeightTolerance = 100, int wallWidthTolerance = 100);

/// <summary>
/// Checks if a player is in a corner
/// </summary>
/// <param name="player">The player to check</param>
/// <param name="wallWidthTolerance">The distance off the walls where the players starts to be considered on the wall</param>
/// <returns>Whether the player is in a corner</returns>
bool isOnCorner(const PlayerData player, int wallWidthTolerance = 100);

/// <summary>
/// Checks if a player is on the ceiling
/// </summary>
/// <param name="player">The player to check</param>
/// <param name="wallHeightTolerance">The distance off the ceiling where the player starts to be considered on the ceiling</param>
/// <param name="wallWidthTolerance">The distance off the walls where the players starts to be considered on the ceiling</param>
/// <returns>Whether the player is on the ceiling</returns>
bool IsOnCeiling(const PlayerData player, int wallHeightTolerance = 100, int wallWidthTolerance = 100);

END_RCF_NS