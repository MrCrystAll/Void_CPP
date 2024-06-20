#include "Rewards/Pinch/CeilingPinch.h"

USE_PINCH_NS

void PinchCeilingSetupReward::Reset(const RLGSC::GameState& initialState)
{
	this->pinchReward.Reset(initialState);
}

float PinchCeilingSetupReward::GetReward(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction)
{
	float reward = 0.0f;
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
				AddLog(reward, "Ground/Behind the ball", config.groundHandling.ballGroundHandling.behindTheBallReward);
			}
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

		AddLog(reward, "Ground/Speed matching", speedMatchingBonus);
		AddLog(reward, "Ground/Wall alignment punishment", wallComparisonPunish);
		AddLog(reward, "Ground/Player distance to ball", -ballDistPunish);
		AddLog(reward, "Ground/Direction matching", directionMatchBonus);

		if (player.ballTouchedStep) {
			AddLog(reward, "Ground/Ball touch", config.groundHandling.touchReward);
		}
	}

	//Wall stuff
	if (std::abs(ball.pos.x) >= RLGSC::CommonValues::SIDE_WALL_X - config.groundHandling.distWallThresh
		and std::abs(agentState.pos.x) >= RLGSC::CommonValues::SIDE_WALL_X - config.groundHandling.distWallThresh
		) {

		if (agentState.pos.z < ball.pos.z and std::abs(agentState.pos.y) > std::abs(ball.pos.y - config.wallHandling.underBallOffsetY) and std::abs(agentState.pos.y) < std::abs(ball.pos.y + config.wallHandling.underBallOffsetY)) {
			AddLog(reward, "Wall/Under the ball", config.wallHandling.underTheBallReward);
		}

		float ballDistPunish = state.ball.pos.Dist(player.carState.pos) / config.wallHandling.ballDistReduction;
		AddLog(reward, "Wall/Agent dist to ball", -ballDistPunish);

		//Reward for ball height ?
		AddLog(reward, "Wall/Ball height", ball.pos.z / RLGSC::CommonValues::CEILING_Z * config.wallHandling.ballHeightW);
	}

	if (player.carState.pos.z > config.ceilingHandling.banZoneHeight and ball.pos.z > config.ceilingHandling.banZoneHeight) {
		if (player.carState.isOnGround) AddLog(reward, "Ceiling/Ground ban", config.ceilingHandling.groundedBan);
		else AddLog(reward, "Ceiling/Off Ground Reward", config.ceilingHandling.ungroundedReward);
	}

	//If within ceiling range
	if (ball.pos.z > RLGSC::CommonValues::CEILING_Z - config.ceilingHandling.distToCeilThresh) {
		//Already good being there, get some snack
		AddLog(reward, "Ceiling/Ceiling zone", config.ceilingHandling.onCeilingReward);

		//Touching the ball and ceiling range ? Sure thing buddy
		if (player.ballTouchedStep) {
			AddLog(reward, "Ceiling/Ceiling pinch", this->pinchReward.GetReward(player, state, prevAction));
		}
	}

	return reward;
}

void PinchCeilingSetupReward::ClearChanges()
{
	float temp;
	AddLog(temp, "Ground/Behind the ball", 0, true);
	AddLog(temp, "Ground/Speed matching", 0, true);
	AddLog(temp, "Ground/Wall alignment punishment", 0, true);
	AddLog(temp, "Ground/Player distance to ball", 0, true);
	AddLog(temp, "Ground/Direction matching", 0, true);
	AddLog(temp, "Ground/Ball touch", 0, true);
	AddLog(temp, "Wall/Under the ball", 0, true);
	AddLog(temp, "Wall/Agent dist to ball", 0, true);
	AddLog(temp, "Wall/Ball height", 0, true);
	AddLog(temp, "Ceiling/Ground ban", 0, true);
	AddLog(temp, "Ceiling/Off Ground Reward", 0, true);
	AddLog(temp, "Ceiling/Off Ground Reward", 0, true);
	AddLog(temp, "Ceiling/Ceiling zone", 0, true);
	AddLog(temp, "Ceiling/Ceiling pinch", 0, true);
	this->pinchReward.ClearChanges();
}

void PinchCeilingSetupReward::Log(RLGPC::Report& report, std::string name, float weight)
{
	LoggableReward::Log(report, name, weight);
	this->pinchReward.Log(report, name, weight);
}