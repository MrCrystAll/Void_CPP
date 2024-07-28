/*****************************************************************//**
 * \file   AbstractRCF.h
 * \brief  The base class for all RCFs
 * 
 * \author Mathieu Suchet
 * \date   July 2024
 *********************************************************************/

#pragma once

#include <Replays/RCF/RCFUtils.h>
#include <RLGymSim_CPP/Utils/Gamestates/GameState.h>
#include <Replays/RCF/StateQueue.h>

START_RCF_NS

using namespace RLGSC;

/// <summary>
/// The base class for all RCFs
/// </summary>
class AbstractRCF {
public:
	/// <summary>
	/// A filter for one player, used to determine if a player matches what the user wants
	/// </summary>
	/// <param name="player">The player to compare</param>
	/// <param name="state">The state it's in</param>
	/// <returns>Whether it matches the user's conditions</returns>
	virtual bool PlayerFilter(const PlayerData player, const GameState state) = 0;

	/// <summary>
	/// A filter for all the players
	/// </summary>
	/// <param name="state">The state to verify</param>
	/// <returns>Whether it matches the user's condition</returns>
	virtual bool AllPlayersFilter(const GameState state);

	/// <summary>
	/// Analyzes all the states and return only the ones matching the user's conditions
	/// </summary>
	/// <param name="states">All the states</param>
	/// <returns>Matching states</returns>
	virtual std::vector<GameState> MatchStates(const std::vector<GameState>& states);
};

/// <summary>
/// A slightly modified rcf containing a queue to keep old states
/// </summary>
class AbstractQueuedRCF: public AbstractRCF {
public:
	StateQueue states;

	AbstractQueuedRCF(const int queueSize) : states(StateQueue(queueSize)) {};

	/// <summary>
	/// A filter for all the players, stores the state in the queue
	/// </summary>
	/// <param name="state">The state to verify</param>
	/// <returns>Whether the state matches the user's condition</returns>
	virtual bool AllPlayersFilter(const GameState state);

	/// <summary>
	/// Analyzes all the states and return only the ones matching the user's conditions, if a state is matched, it'll return the first state of the queue (oldest state in the queue)
	/// </summary>
	/// <param name="states">All the states</param>
	/// <returns>Matching states</returns>
	virtual std::vector<GameState> MatchStates(const std::vector<GameState>& states);
};

END_RCF_NS