#pragma once
#include <RLGymSim_CPP/Utils/OBSBuilders/OBSBuilder.h>
#include "../RLGymPPO_CPP/RLGymSim_CPP/RocketSim/src/Sim/BallPredTracker/BallPredTracker.h"

class OnesObs : public RLGSC::OBSBuilder {
public:
	// Inherited via OBSBuilder
	RLGSC::FList BuildOBS(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction) override;
};

using namespace RLGSC;

// https://github.com/AechPro/rocket-league-gym-sim/blob/main/rlgym_sim/utils/obs_builders/default_obs.py
namespace RLGSC {
	class DefaultOBS : public OBSBuilder {
	public:

		Vec posCoef;
		float velCoef, angVelCoef;
		DefaultOBS(
			Vec posCoef = Vec(1 / CommonValues::SIDE_WALL_X, 1 / CommonValues::BACK_WALL_Y, 1 / CommonValues::CEILING_Z),
			float velCoef = 1 / CommonValues::CAR_MAX_SPEED,
			float angVelCoef = 1 / CommonValues::CAR_MAX_ANG_VEL
		) : posCoef(posCoef), velCoef(velCoef), angVelCoef(angVelCoef) {

		}

		void AddPlayerToOBS(FList& obs, const PlayerData& player, bool inv);

		virtual FList BuildOBS(const PlayerData& player, const GameState& state, const Action& prevAction);
	};
}

class BallPredObs : public RLGSC::DefaultOBS {
public:
	BallPredObs(
		Vec posCoef = Vec(1 / CommonValues::SIDE_WALL_X, 1 / CommonValues::BACK_WALL_Y, 1 / CommonValues::CEILING_Z),
		float velCoef = 1 / CommonValues::CAR_MAX_SPEED,
		float angVelCoef = 1 / CommonValues::CAR_MAX_ANG_VEL,
		float predictionTime = 120
	) : DefaultOBS(posCoef, velCoef, angVelCoef), predTime(predictionTime)
	{};

	virtual FList BuildOBS(const PlayerData& player, const GameState& state, const Action& prevAction);
	virtual void PreStep(const GameState& state);
	virtual void Reset(const GameState& initialState);

private:
	BallPredTracker* bpt;
	float predTime;
};

class DefaultPaddedObs : public RLGSC::DefaultOBS {
public:
	DefaultPaddedObs(
		Vec posCoef = Vec(1 / CommonValues::SIDE_WALL_X, 1 / CommonValues::BACK_WALL_Y, 1 / CommonValues::CEILING_Z),
		float velCoef = 1 / CommonValues::CAR_MAX_SPEED,
		float angVelCoef = 1 / CommonValues::CAR_MAX_ANG_VEL,
		int teamSize = 3,
		bool spawnOpponents = true
	) : DefaultOBS(posCoef, velCoef, angVelCoef), teamSize(teamSize), spawnOpponents(spawnOpponents)
	{};

	virtual FList BuildOBS(const PlayerData& player, const GameState& state, const Action& prevAction);
private:

	void AddEmptyPlayerTo(FList& obs);

	int teamSize;
	bool spawnOpponents;

	int PLAYER_SIZE = 19;
};