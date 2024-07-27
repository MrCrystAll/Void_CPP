#pragma once

#include <Replays/RCF/RCFUtils.h>
#include <RLGymSim_CPP/Utils/Gamestates/GameState.h>
#include <Replays/RCF/StateQueue.h>

START_RCF_NS

using namespace RLGSC;

class AbstractRCF {
public:
	virtual bool PlayerFilter(const PlayerData player, const GameState state) = 0;
	virtual bool AllPlayersFilter(const GameState state);
	virtual std::vector<GameState> MatchStates(const std::vector<GameState>& states);
};

class AbstractQueuedRCF: public AbstractRCF {
public:
	StateQueue states;

	AbstractQueuedRCF(const int queueSize) : states(StateQueue(queueSize)) {};

	virtual bool AllPlayersFilter(const GameState state);
	virtual std::vector<GameState> MatchStates(const std::vector<GameState>& states);
};

END_RCF_NS