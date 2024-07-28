/*****************************************************************//**
 * \file   StateQueue.h
 * \brief  A queue of states used by RCFs
 * 
 * \author Mathieu Suchet
 * \date   July 2024
 *********************************************************************/

#pragma once

#include <Replays/RCF/RCFUtils.h>
#include <RLGymSim_CPP/Utils/Gamestates/GameState.h>

START_RCF_NS

using namespace RLGSC;

/// <summary>
/// A queue of states used by RCFs
/// </summary>
class StateQueue{
private:
	std::queue<GameState> queue;
	const int queueSize;

public:
	StateQueue(const int queueSize) : queueSize(queueSize), queue(std::queue<GameState>()) {};

	/// <summary>
	/// Push at the end of the queue
	/// </summary>
	/// <param name="state">The state to push</param>
	void PushToQueue(const GameState state);

	/// <summary>
	/// Gets the first element of the queue
	/// </summary>
	/// <returns>The first element of the queue</returns>
	const GameState GetFront() const;

	/// <summary>
	/// Clears the queue
	/// </summary>
	void ClearQueue();

	/// <summary>
	/// Checks if the queue is full
	/// </summary>
	/// <returns>Whether the queue is full</returns>
	const bool IsFull();
};

END_RCF_NS