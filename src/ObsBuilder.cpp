#include "ObsBuilder.h"
#include "../RLGymPPO_CPP/RLGymSim_CPP/RocketSim/src/Sim/BallPredTracker/BallPredTracker.h"

USE_OBS_BUILDER_NS;

using namespace RLGSC;

RLGSC::FList OnesObs::BuildOBS(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction)
{
    RLGSC::FList result = {};
    for (int i = 0; i < 70; i++) {
        result.push_back(1);
    }

    return result;
};

FList BallPredObs::BuildOBS(const PlayerData& player, const GameState& state, const Action& prevAction)
{
	FList obs = DefaultOBS::BuildOBS(player, state, prevAction);

	BallState ballPredState = this->bpt->GetBallStateForTime(predTime);

	obs += ballPredState.pos * posCoef;
	obs += ballPredState.vel * velCoef;
	obs += ballPredState.angVel * angVelCoef;

	return obs;
}

void BallPredObs::PreStep(const GameState& state)
{
	DefaultOBS::PreStep(state);
	this->bpt->UpdatePredFromArena(state.lastArena);
}

void BallPredObs::Reset(const GameState& initialState)
{
	DefaultOBS::Reset(initialState);
	if (bpt == nullptr) {
		bpt = new BallPredTracker(initialState.lastArena, predTime);
	}
}

FList DefaultPaddedObs::BuildOBS(const PlayerData& player, const GameState& state, const Action& prevAction)
{
	FList obs = DefaultOBS::BuildOBS(player, state, prevAction);

	int nToPad = teamSize * (this->spawnOpponents + 1) - state.players.size();

	for (int i = 0; i < nToPad; i++) {
		AddEmptyPlayerTo(obs);
	}

	return obs;
}

void DefaultPaddedObs::AddEmptyPlayerTo(FList& obs)
{
	std::vector<float> emptyPlayer(PLAYER_SIZE, 0.0f);
	obs.insert(obs.end(), emptyPlayer.begin(), emptyPlayer.end());
}
