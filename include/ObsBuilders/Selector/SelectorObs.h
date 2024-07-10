#pragma once
#include <RLGymSim_CPP/Utils/OBSBuilders/OBSBuilder.h>
#include <Utils/Selector/SelectorChoice.h>

class SelectorObs: public RLGSC::OBSBuilder {
public:

	SelectorObs(RLGSC::OBSBuilder* obsBuilder, SelectorChoice* choice) : obsBuilder(obsBuilder), choice(choice) {};

	// Inherited via OBSBuilder
	RLGSC::FList BuildOBS(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction) override;
	virtual void Reset(const RLGSC::GameState& initialState) override;
	virtual void PreStep(const RLGSC::GameState& state) override;

private:
	RLGSC::OBSBuilder* obsBuilder;
	SelectorChoice* choice;
};
