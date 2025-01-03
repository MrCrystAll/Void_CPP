#pragma once

#include <RLGymSim_CPP/Utils/OBSBuilders/DefaultOBS.h>
#include <Recovery/RecoveryUtils.h>

START_RECOVERY_OB_NS

class DashObsBuilder: public RLGSC::DefaultOBS {
public:
	int maxPlayers;

	DashObsBuilder(
		int maxPlayers,
		Vec posCoef = Vec(1 / RLGSC::CommonValues::SIDE_WALL_X, 1 / RLGSC::CommonValues::BACK_WALL_Y, 1 / RLGSC::CommonValues::CEILING_Z),
		float velCoef = 1 / RLGSC::CommonValues::CAR_MAX_SPEED,
		float angVelCoef = 1 / RLGSC::CommonValues::CAR_MAX_ANG_VEL
	) : DefaultOBS(posCoef, velCoef, angVelCoef), maxPlayers(maxPlayers) {

	};

	void AddPlayerToOBS(RLGSC::FList& obs, const RLGSC::PlayerData& player, bool inv);
	virtual void PreStep(const RLGSC::GameState& state) override;
	// Inherited via OBSBuilder
	RLGSC::FList BuildOBS(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction) override;
	virtual void Reset(const RLGSC::GameState& initialState) override;
private:
	std::map<int, int> dashStreaks = {};
	std::map<int, bool> lastIsOnGround = {};
	std::map<int, bool> lastHasFlipped = {};
};

END_RECOVERY_NS