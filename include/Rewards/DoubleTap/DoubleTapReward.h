#pragma once

#include "LoggedCombinedReward.h"
#include "Utils/DoubleTapUtils.h"

START_DT_NS
class DoubleTapReward : public LoggableReward {
public:

	struct BallHandling {
		//Minimum distance to start going for aim
		float minDistToBall = 350.0f;

		//Reduction for ball dist to agent
		float distBallReduction = 50.0f;

		//Touch weight
		float touchW = 10.0f;
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
	};

	struct DoubleTapArgs {
		BallHandling ballHandling;
		GoalHandling goalHandling;
	};

	DoubleTapReward(DoubleTapArgs args) : config(args), scoreLine(RLGSC::ScoreLine()) {};
	virtual void ClearChanges();
	virtual float GetReward(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction);

private:
	RLGSC::ScoreLine scoreLine;
	DoubleTapArgs config;
};
END_DT_NS