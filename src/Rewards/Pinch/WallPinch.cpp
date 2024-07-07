#include "Rewards/Pinch/WallPinch.h"

USE_PINCH_NS

void PinchWallSetupReward::Reset(const RLGSC::GameState& initialState)
{
	this->pinchReward.Reset(initialState);
}

float PinchWallSetupReward::GetReward(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction)
{
	auto ballDir = state.ball.vel.Normalized();
	auto agentDir = player.carState.vel.Normalized();
	short targetDir = ballDir.x < 0 ? -1 : 1;

	bool corner = false;

	float wallInterceptionY = (ballDir.y / ballDir.x) * (RLGSC::CommonValues::SIDE_WALL_X * targetDir - state.ball.pos.x) + state.ball.pos.y;
	Vec interceptPoint = Vec(RLGSC::CommonValues::SIDE_WALL_X * targetDir, wallInterceptionY, 0);
	Vec intersectionPoint = GetCornerIntersection(ballDir.x, ballDir.y, state.ball.pos.x, state.ball.pos.y);

	//First things first, is the intersect point valid
	if (std::abs(intersectionPoint.x) <= RLGSC::CommonValues::SIDE_WALL_X or std::abs(intersectionPoint.x) >= RLGSC::CommonValues::SIDE_WALL_X - 1152) {
		interceptPoint = intersectionPoint;
		corner = true;
	};

	float distToIntercept = interceptPoint.Dist2D(state.ball.pos);
	if (distToIntercept < config.distancesWallSetup.pinchDistance or state.ball.pos.z > 200) { interceptPoint = lastIntercept; }
	distToIntercept = interceptPoint.Dist2D(state.ball.pos);

	Vec agentToBall = state.ball.pos - player.carState.pos;
	float agentToBallDist = agentToBall.Length();



	//First thing, detect if we trigger based on ball dist to wall intercept
	if (state.ball.pos.Dist2D(interceptPoint) < config.distancesWallSetup.maxDistToTrigger and player.carState.pos.Dist(state.ball.pos) < config.distancesWallSetup.maxDistToTrigger) {
		//Testing if before creeping
		if (state.ball.pos.Dist2D(interceptPoint) > config.distancesWallSetup.creepingDistance) {
			//Match direction and speed
			float speedDiff = std::max(0.01f, std::abs(state.ball.vel.Dist(player.carState.vel)) / RLGSC::CommonValues::SUPERSONIC_THRESHOLD);
			float directionSimilarity = agentDir.Dot(ballDir);

			if (agentToBallDist < config.ballWallHandling.agentDistToBallThresh) {
				if (abs(agentToBall.x) < config.ballWallHandling.ballOffsetX and abs(agentToBall.y) < config.ballWallHandling.ballOffsetY) {
					reward += {config.ballWallHandling.behindTheBallReward, "Behind the ball"};
				}
			}

			float speedMatchingBonus = 1 / pow(speedDiff, 0.1f) * config.ballWallHandling.speedMatchW;
			float ballDistPunish = state.ball.pos.Dist(player.carState.pos) / config.ballWallHandling.ballDistReduction;
			float directionMatchBonus = directionSimilarity >= config.similarityWallSetup.similarityBallAgentThresh ? config.similarityWallSetup.similarityBallAgentReward : 0;

			reward += {speedMatchingBonus, "Speed matching"};
			reward += {-ballDistPunish, "Player distance to ball"};
			reward += {directionMatchBonus, "Direction matching"};
		}

		//Creeping distance
		else {
			//Being in this zone is good already, get some snacks
			reward += {config.groundWallSetupHandling.creepingDistanceReward, "Creeping distance reward"};


			float agentDistToIntercept = Vec(RLGSC::CommonValues::SIDE_WALL_X * targetDir, player.carState.pos.y, 0).Dist2D(player.carState.pos);

			//Being in the ground ban zone
			if (agentDistToIntercept <= config.distancesWallSetup.groundBanDistance) {
				//If grounded, get spanked, else, you're doing great
				float groundChange = player.carState.isOnGround ? config.groundWallSetupHandling.groundBanPunishment : config.groundWallSetupHandling.groundBanReward;
				reward += {groundChange, "Creeping grounded"};


				//If still relatively far
				if (state.ball.pos.Dist(player.carState.pos) > config.flipHandlingWallSetup.maxDistance + RLGSC::CommonValues::BALL_RADIUS) {
					//Don't use your flip or punished
					float flipChange = player.hasFlip ? config.flipHandlingWallSetup.hasFlipReward : config.flipHandlingWallSetup.hasFlipPunishment;
					reward += {flipChange, "Creeping flip"};

				}

				//Accel reward on ball hit
				else if (player.ballTouchedStep and state.ball.pos.z >= config.wallHandling.wallMinHeightToPinch) {
					reward += {this->pinchReward.GetReward(player, state, prevAction), "Pinch reward"};

				}
			}
		}
	}

	lastIntercept = interceptPoint;


	return this->ComputeReward();
}

float PinchWallSetupReward::Corner(float x, short xOrientation, short yOrientation)
{
	return xOrientation * x + 8064 * yOrientation;
}

Vec PinchWallSetupReward::GetCornerIntersection(short xFwd, short yFwd, float xPos, float yPos)
{
	float xResult = 0;
	float yResult = 0;

	short xOrientation = 0;
	short yOrientation = 0;

	//Right
	if (xFwd > 0) {

		//Top
		if (yFwd > 0) {
			xOrientation = -1;
			yOrientation = 1;
		}
		else {
			xOrientation = 1;
			yOrientation = -1;
		}
	}
	else {
		//Bottom
		if (yFwd < 0) {
			xOrientation = -1;
			yOrientation = -1;
		}
		else {
			xOrientation = 1;
			yOrientation = 1;
		}
	}

	xResult = (xFwd * (8064 * yOrientation - yPos) + yFwd * xPos) / (yFwd - xFwd * xOrientation);

	yResult = Corner(xResult, xOrientation, yOrientation);
	return Vec(xResult, yResult, 0.0f);
}

void PinchWallSetupReward::LogAll(Report& report, bool final, std::string name, float weight)
{
	LoggableReward::LogAll(report, final, name, weight);
	this->pinchReward.LogAll(report, final, "Pinch", weight);
}