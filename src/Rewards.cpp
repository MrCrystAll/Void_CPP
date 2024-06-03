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

			float speedMatchingBonus = 1 / pow(speedDiff, 0.1f) * ballHandling.speedMatchW;
			float ballDistPunish = state.ball.pos.Dist(player.carState.pos) / ballHandling.ballDistReduction;
			float directionMatchBonus = directionSimilarity >= similarity.similarityBallAgentThresh ? similarity.similarityBallAgentReward : 0;

			AddChange(reward, "Speed matching", speedMatchingBonus);
			AddChange(reward, "Player distance to ball", -ballDistPunish);
			AddChange(reward, "Direction matching", directionMatchBonus);
		}

		//Creeping distance
		else {
			//Being in this zone is good already, get some snacks
			AddChange(reward, "Creeping distance reward", groundHandling.creepingDistanceReward);

			float agentDistToIntercept = Vec(RLGSC::CommonValues::SIDE_WALL_X * targetDir, player.carState.pos.y, 0).Dist2D(player.carState.pos);

			//Being in the ground ban zone
			if (agentDistToIntercept <= distances.groundBanDistance) {
				//If grounded, get spanked, else, you're doing great
				float groundChange = player.carState.isOnGround ? groundHandling.groundBanPunishment : groundHandling.groundBanReward;
				AddChange(reward, "Creeping grounded", groundChange);

				//If still relatively far
				if (state.ball.pos.Dist(player.carState.pos) > flipHandling.maxDistance + RLGSC::CommonValues::BALL_RADIUS) {
					//Don't use your flip or punished
					float flipChange = player.hasFlip ? flipHandling.hasFlipReward : flipHandling.hasFlipPunishment;
					AddChange(reward, "Creeping flip", flipChange);
				}
				else {
					if (player.ballTouchedStep) {
						//You still got your flip when hitting the ball ? punished, else good
						float flipBallChange = !player.hasFlip ? flipHandling.hasFlipPunishmentWhenBall : flipHandling.hasFlipPunishmentWhenBall;
						AddChange(reward, "Ball flip", flipBallChange);
					}
				}
			}

			//Accel reward on ball hit
			if (player.ballTouchedStep and state.ball.pos.z >= wallHandling.wallMinHeightToPinch) {
				float ballAccel = (state.ball.vel.Length2D() - lastBallSpeed) / RLConst::BALL_MAX_SPEED;

				AddChange(reward, "Ball touch", ballHandling.touchW);
				AddChange(reward, "Ball accel", ballAccel * ballHandling.ballVelW);				
			}
		}
	}

	lastBallSpeed = state.ball.vel.Length2D();


    return reward;
}

void PinchReward::ClearChanges()
{
	float temp = 0;
	LoggableReward::ClearChanges();
	AddChange(temp, "Speed matching", 0, true);
	AddChange(temp, "Player distance to ball", 0, true);
	AddChange(temp, "Direction matching", 0, true);
	AddChange(temp, "Creeping distance reward", 0, true);
	AddChange(temp, "Creeping grounded", 0, true);
	AddChange(temp, "Creeping flip", 0, true);
	AddChange(temp, "Ball flip", 0, true);
	AddChange(temp, "Ball touch", 0, true);
	AddChange(temp, "Ball accel", 0, true);

}
