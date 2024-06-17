#pragma once
#include <RLGymSim_CPP/Utils/OBSBuilders/OBSBuilder.h>

class OnesObs : public RLGSC::OBSBuilder {
public:
	// Inherited via OBSBuilder
	RLGSC::FList BuildOBS(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction) override;
};