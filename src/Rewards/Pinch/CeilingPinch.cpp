#include "Rewards/Pinch/CeilingPinch.h"

USE_PINCH_NS

void PinchCeilingSetupReward::Reset(const RLGSC::GameState& initialState)
{
	this->pinchReward.Reset(initialState);
}

float PinchCeilingSetupReward::GetReward(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction)
{
	auto ballDir = state.ball.vel.Normalized();
	auto agentDir = player.carState.vel.Normalized();
	short targetDir = ballDir.x < 0 ? -1 : 1;

	RLGSC::PhysObj ball = state.ball;
	RocketSim::CarState agentState = player.carState;

	//Ground to wall
	if (std::abs(ball.pos.x) < RLGSC::CommonValues::SIDE_WALL_X - config.groundHandling.distWallThresh
		and agentState.pos.z < (config.groundHandling.groundThresh + 17)
		and ball.pos.z < (RLGSC::CommonValues::BALL_RADIUS + config.groundHandling.groundThresh)
		) {

		Vec agentToBall = ball.pos - agentState.pos;
		float agentToBallDist = agentToBall.Length();

		if (agentToBallDist < config.groundHandling.ballGroundHandling.agentDistToBallThresh) {
			if (abs(agentToBall.x) < config.groundHandling.ballGroundHandling.ballOffsetX and abs(agentToBall.y) < config.groundHandling.ballGroundHandling.ballOffsetY) {
				reward += {config.groundHandling.ballGroundHandling.behindTheBallReward, "Ground/Behind the ball"};
;			}
		}

		float speedDiff = std::max(0.01f, std::abs(state.ball.vel.Dist(player.carState.vel)) / RLGSC::CommonValues::SUPERSONIC_THRESHOLD);
		float directionSimilarity = agentDir.Dot(ballDir);

		//Zooming like the ball is zooming, i like it
		float speedMatchingBonus = 1 / pow(speedDiff, 0.1f) * config.groundHandling.agentSimilarity.speedMatchW;

		//Going too far away from the ball ? I don't like it
		float ballDistPunish = state.ball.pos.Dist(player.carState.pos) / config.groundHandling.ballGroundHandling.ballDistReduction;

		//Going parallel to the wall, i will send you to Jesus
		float wallComparisonPunish = Vec(targetDir, 0, 0).Dot(agentState.vel.Normalized()) > config.groundHandling.wallAgentAndBallThreshold and Vec(targetDir, 0, 0).Dot(ball.vel.Normalized()) > config.groundHandling.wallAgentAndBallThreshold ? 0 : config.groundHandling.wallAgentAndBallPunishment;

		//You're matching the ball's direction ? Here, a snack
		float directionMatchBonus = directionSimilarity >= config.groundHandling.agentSimilarity.similarityBallAgentThresh ? config.groundHandling.agentSimilarity.similarityBallAgentReward : 0;

		reward += {speedMatchingBonus, "Ground/Behind the ball"};
		reward += {wallComparisonPunish, "Ground/Wall alignment punishment"};
		reward += {-ballDistPunish, "Ground/Player distance to ball"};
		reward += {directionMatchBonus, "Ground/Direction matching"};

		if (player.ballTouchedStep) {
			reward += {config.groundHandling.touchReward, "Ground/Ball touch"};

		}
	}

	//Wall stuff
	if (std::abs(ball.pos.x) >= RLGSC::CommonValues::SIDE_WALL_X - config.groundHandling.distWallThresh
		and std::abs(agentState.pos.x) >= RLGSC::CommonValues::SIDE_WALL_X - config.groundHandling.distWallThresh
		) {

		if (agentState.pos.z < ball.pos.z and std::abs(agentState.pos.y) > std::abs(ball.pos.y - config.wallHandling.underBallOffsetY) and std::abs(agentState.pos.y) < std::abs(ball.pos.y + config.wallHandling.underBallOffsetY)) {
			reward += {config.wallHandling.underTheBallReward, "Wall/Under the ball"};
		}

		float ballDistPunish = state.ball.pos.Dist(player.carState.pos) / config.wallHandling.ballDistReduction;
		reward += {-ballDistPunish, "Wall/Agent dist to ball"};

		//Reward for ball height ?
		reward += {ball.pos.z / RLGSC::CommonValues::CEILING_Z * config.wallHandling.ballHeightW, "Wall/Ball height"};
	}

	if (player.carState.pos.z > config.ceilingHandling.banZoneHeight and ball.pos.z > config.ceilingHandling.banZoneHeight) {
		if (player.carState.isOnGround) {
			reward += {config.ceilingHandling.groundedBan, "Ceiling/Ground ban"};
		}
		
		else{
			reward += {config.ceilingHandling.ungroundedReward, "Ceiling/Off Ground Reward"};
		}
	}

	//If within ceiling range
	if (ball.pos.z > RLGSC::CommonValues::CEILING_Z - config.ceilingHandling.distToCeilThresh) {
		//Already good being there, get some snack
		reward += {config.ceilingHandling.onCeilingReward, "Ceiling/Ceiling zone"};

		//Touching the ball and ceiling range ? Sure thing buddy
		if (player.ballTouchedStep) {
			reward += {this->pinchReward.GetReward(player, state, prevAction), "Ceiling/Ceiling pinch"};
		}
	}

	return reward.value;
}

void PinchCeilingSetupReward::LogAll(Report& report, bool final, std::string name, float weight)
{
	LoggableReward::LogAll(report, final, name, weight);
	this->pinchReward.LogAll(report, final, "Pinch", weight);
}