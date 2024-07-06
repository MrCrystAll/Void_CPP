/*****************************************************************//**
 * \file   DoubleTapReward.h
 * \brief  Contains double tap reward and a reward that uses it
 * 
 * \author Mathieu Suchet
 * \date   July 2024
 *********************************************************************/
#pragma once

#include <Logging/LoggableReward.h>
#include "Utils/DoubleTapUtils.h"

START_DT_NS
class DoubleTapReward : public LoggableReward {
public:

	struct BallHandling {
		//Minimum distance to start going for aim
		float minDistToBall = 350.0f;

		//Reduction for ball dist to agent
		float distBallReduction = 50.0f;

		//Maximum distance to trigger touch from bb
		float maxDistFromBB = 500.0f;

		//Minimum height to trigger touch
		float minHeightToTrigger = RLGSC::CommonValues::GOAL_HEIGHT + RLGSC::CommonValues::BALL_RADIUS;

		//Touch weight
		float touchW = 10.0f;

		//Minimum similarity required to ball to trigger a reward
		float minSimilarityAgentBall = 0.7f;

		//Similarity agent ball weight
		float similarityAgentBallW = 10.0f;
	};

	struct GoalHandling {
		//Minimum similarity to trigger goal dir
		float minSimilarityGoalBall = 0.5f;

		//Weight for similarity goal ball dir
		float similarityGoalBallW = 10.0f;

		//Weight for goal
		float goalW = 100.0f;

		//Goal speed weight
		float goalSpeedW = 10.0f;

		//Concede weight
		float concedeW = 1.0f;
	};

	struct DoubleTapArgs {
		BallHandling ballHandling;
		GoalHandling goalHandling;
	};

	struct PhysObjLastHit {
		PlayerData physObj;
		int lastStepHit = 0;
	};

	struct PhysObjHandling {
		std::map<int, PhysObjLastHit> allCurrentHits;

		void Clear();
		void Reset(const RLGSC::GameState& initialState);
		void Update(const RLGSC::GameState& initialState, const RLGSC::PlayerData& player, const int nbSteps);
		const PlayerData* GetLatestHit(const Team team);
	};

	DoubleTapReward(std::string name, DoubleTapArgs args) : config(args), LoggableReward(name), scoreLine(RLGSC::ScoreLine()) {};
	virtual void Reset(const RLGSC::GameState& initialState);
	virtual RLGSC::FList GetAllRewards(const RLGSC::GameState& state, const RLGSC::ActionSet& prevActions, bool final) override;
	virtual float GetReward(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction);

private:
	ScoreLine scoreLine;
	DoubleTapArgs config;
	PhysObjHandling physHandling;
	int nbSteps = 0;
};


class UseDTReward : public LoggableReward{
public:
	UseDTReward(std::string name, DoubleTapReward::DoubleTapArgs dtConfig) : dtConfig(dtConfig), LoggableReward(name), dtReward(DoubleTapReward("DT", dtConfig)) {};
	virtual void Reset(const RLGSC::GameState& initialState) override;
	virtual void LogAll(Report& report, bool final, std::string name = "", float weight = 1.f);
	virtual void PreStep(const GameState& state);
	virtual std::vector<float> GetAllRewards(const GameState& state, const ActionSet& prevActions, bool final);
protected:
	DoubleTapReward::DoubleTapArgs dtConfig;
	DoubleTapReward dtReward;
};

END_DT_NS