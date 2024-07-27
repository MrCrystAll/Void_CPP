#pragma once

#include <Replays/RCF/RCFUtils.h>
#include <RLGymSim_CPP/Utils/Gamestates/GameState.h>

START_RCF_NS

using namespace RLGSC;

class StateQueue{
private:
	std::queue<GameState> queue;
	const int queueSize;

public:
	StateQueue(const int queueSize) : queueSize(queueSize), queue(std::queue<GameState>()) {};

	void PushToQueue(const GameState state);
	const GameState GetFront() const;
	void ClearQueue();
	const bool IsFull();
};

END_RCF_NS