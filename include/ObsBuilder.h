/*****************************************************************//**
 * \file   ObsBuilder.h
 * \brief  Custom obs builders
 * 
 * \author Mathieu Suchet
 * \date   July 2024
 *********************************************************************/

#pragma once
#include <RLGymSim_CPP/Utils/OBSBuilders/OBSBuilder.h>
#include <RLGymSim_CPP/Utils/OBSBuilders/DefaultOBS.h>
#include "../RLGymPPO_CPP/RLGymSim_CPP/RocketSim/src/Sim/BallPredTracker/BallPredTracker.h"
#include <Utils/VoidUtils.h>

START_OBS_BUILDER_NS

/**
 * @brief An observation build that fills the obs with 1s
 */
class OnesObs : public RLGSC::OBSBuilder {
public:
	// Inherited via OBSBuilder
	RLGSC::FList BuildOBS(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction) override;
};

using namespace RLGSC;

class LockedDefaultObs : public RLGSC::DefaultOBS {
public:
	Vec posCoef;
	float velCoef, angVelCoef;
	int teamSize = 3;
	bool spawnOpponents = true;
	LockedDefaultObs(
		int teamSize = 3,
		bool spawnOpponents = true,
		Vec posCoef = Vec(1 / CommonValues::SIDE_WALL_X, 1 / CommonValues::BACK_WALL_Y, 1 / CommonValues::CEILING_Z),
		float velCoef = 1 / CommonValues::CAR_MAX_SPEED,
		float angVelCoef = 1 / CommonValues::CAR_MAX_ANG_VEL
	) : DefaultOBS(posCoef, velCoef, angVelCoef), teamSize(teamSize), spawnOpponents(spawnOpponents) {

	}

	virtual FList BuildOBS(const PlayerData& player, const GameState& state, const Action& prevAction);
};

/**
 * @brief Obs with ball prediction
 */
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

/**
 * @brief Padded obs for multi gamemode support
 */
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

END_OBS_BUILDER_NS
