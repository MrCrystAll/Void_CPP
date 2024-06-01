#include "Rewards.h"

PinchReward::PinchReward(
	PinchArgs args
): 
    distances(args.distances),
	flipHandling(args.flipHandling),
	ballHandling(args.ballHandling),
	similarity(args.similarity),
	groundHandling(args.groundHandling),
	wallHandling(args.wallHandling)
{
	this->lastBallSpeed = 0;
}

void PinchReward::Reset(const RLGSC::GameState& initialState)
{
	this->lastBallSpeed = 0;
}

float PinchReward::GetReward(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction)
{
	//REWARD
	float reward = 0.f;

	auto ballDir = state.ball.pos.Normalized();
	auto agentDir = player.carState.vel.Normalized();
	short targetDir = ballDir.x < 0 ? -1 : 1;

	float wallInterceptionY = (ballDir.y / ballDir.x) * (RLGSC::CommonValues::SIDE_WALL_X * targetDir - state.ball.pos.x) + state.ball.pos.y;
	Vec interceptPoint = Vec(RLGSC::CommonValues::SIDE_WALL_X * targetDir, wallInterceptionY, 0);
	
	//First thing, detect if we trigger based on ball dist to wall intercept
	if (state.ball.pos.Dist2D(interceptPoint) < distances.maxDistToTrigger) {
		//Testing if before creeping
		if (state.ball.pos.Dist2D(interceptPoint) > distances.creepingDistance) {
			//Match direction and speed
			float speedDiff = std::max(0.01f, std::abs(state.ball.vel.Dist(player.carState.vel)) / RLGSC::CommonValues::SUPERSONIC_THRESHOLD);
			float directionSimilarity = agentDir.Dot(ballDir);

			reward += 1 / pow(speedDiff, 0.1f) * ballHandling.speedMatchW;
			reward -= state.ball.pos.Dist(player.carState.pos) / ballHandling.ballDistReduction;
			reward += directionSimilarity >= similarity.similarityBallAgentThresh ? similarity.similarityBallAgentReward : 0;
		}

		//Creeping distance
		else {
			//Being in this zone is good already, get some snacks
			reward += groundHandling.creepingDistanceReward;

			float agentDistToIntercept = Vec(RLGSC::CommonValues::SIDE_WALL_X * targetDir, player.carState.pos.y, 0).Dist2D(player.carState.pos);

			//Being in the ground ban zone
			if (agentDistToIntercept <= distances.groundBanDistance) {
				//If grounded, get spanked, else, you're doing great
				reward += player.carState.isOnGround ? groundHandling.groundBanPunishment : groundHandling.groundBanReward;

				//If still relatively far
				if (state.ball.pos.Dist(player.carState.pos) > flipHandling.maxDistance + RLGSC::CommonValues::BALL_RADIUS) {
					//Don't use your flip or punished
					reward += player.hasFlip ? flipHandling.hasFlipReward : flipHandling.hasFlipPunishment;
				}
				else {
					if (player.ballTouchedStep) {
						//You still got your flip when hitting the ball ? punished, else good
						reward += !player.hasFlip ? flipHandling.hasFlipPunishmentWhenBall : flipHandling.hasFlipPunishmentWhenBall;
					}
				}
			}

			//Accel reward on ball hit
			if (player.ballTouchedStep and state.ball.pos.z >= wallHandling.wallMinHeightToPinch) {
				float ballAccel = (state.ball.vel.Length2D() - lastBallSpeed) / RLConst::BALL_MAX_SPEED;
				reward += ballHandling.touchW;
				reward += ballAccel * ballHandling.ballVelW;
				
			}
		}


	}


	lastBallSpeed = state.ball.vel.Length2D();


    return reward;
}
