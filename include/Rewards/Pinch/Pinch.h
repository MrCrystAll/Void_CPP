#pragma once

#include "../../LoggedCombinedReward.h"
#include "PinchUtils.h"

START_PINCH_NS

class PinchReward : public LoggableReward {
public:


	//All the ball handling stuff
	struct BallHandling {
		//Acceleration weight
		float ballVelW = 1.0f;

		//Touch weight
		float touchW = 1.0f;

		//Minimum similarity to match goal direction
		float goalDirectionSimilarity = 0.8f;

		//Reward for going towards the net
		float goalDirectionW = 3.0f;
	};

	struct PinchArgs {
		BallHandling ballHandling = {};
	};

	PinchReward(
		PinchArgs args
	);

	virtual void Reset(const RLGSC::GameState& initialState);
	virtual float GetReward(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction);
	virtual void ClearChanges() override;
private:
	PinchArgs config;

	float lastBallSpeed = 0;
};

END_PINCH_NS