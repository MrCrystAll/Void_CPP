#include "ObsBuilders/Selector/SelectorObs.h"

USE_SELECTOR_NS;

RLGSC::FList SelectorObs::BuildOBS(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction)
{
	return this->obsBuilder->BuildOBS(player, state, prevAction);
}

void SelectorObs::Reset(const RLGSC::GameState& initialState)
{
	this->obsBuilder->Reset(initialState);
	for (Submodel& m : this->choice->GetSubmodels()) {
		m.GetObsBuilder()->Reset(initialState);
	}
}

void SelectorObs::PreStep(const RLGSC::GameState& state)
{
	this->obsBuilder->PreStep(state);
	for (Submodel& m : this->choice->GetSubmodels()) {
		m.GetObsBuilder()->PreStep(state);
	}
}
