#include "Rewards/DoubleTap/DoubleTapReward.h"

USE_DT_NS

void DoubleTapReward::ClearChanges()
{
	float temp;
	AddLog(temp, "Going towards the ball", 0, true);
	AddLog(temp, "Ball touch", 0, true);
	AddLog(temp, "Ball towards goal", 0, true);
	AddLog(temp, "Goal speed", 0, true);
	AddLog(temp, "Goal", 0, true);
	AddLog(temp, "Concede", 0, true);
}
void DoubleTapReward::Reset(const GameState& initialState)
{
	this->physHandling.Reset(initialState);
	this->nbSteps = 0;
}
RLGSC::FList DoubleTapReward::GetAllRewards(const RLGSC::GameState& state, const RLGSC::ActionSet& prevActions, bool final)
{
	this->nbSteps++;
	return LoggableReward::GetAllRewards(state, prevActions, final);
}
;

float DoubleTapReward::GetReward(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction)
{
	float reward = 0.0f;
	this->physHandling.Update(state, player, this->nbSteps);

	//Ball stuff
	Vec ballDir = state.ball.vel.Normalized();
	Vec goalTarget = Vec(0.0f, player.team == Team::BLUE ? RLGSC::CommonValues::BACK_WALL_Y : -RLGSC::CommonValues::BACK_WALL_Y, 0.0f);
	float similarityGoalBall = (state.ball.pos - goalTarget).Normalized().Dot(ballDir);

	//Agent stuff
	Vec agentToBall = state.ball.pos - player.carState.pos;
	Vec agentToBallDir = agentToBall.Normalized();
	Vec agentDir = player.carState.vel.Normalized();

	float agentToBallDist = agentToBall.Length();
	float similarityAgentBall = agentDir.Dot(ballDir);

	//So uuuuuh, first, agent dist to ball until the bot gets relatively close
	if (agentToBallDist > config.ballHandling.minDistToBall) {
		//Punish that boy for being far away from the ball
		AddLog(reward, "Distance to ball", -agentToBallDist / config.ballHandling.distBallReduction);
		AddLog(reward, "Going towards the ball", (similarityAgentBall > config.ballHandling.minSimilarityAgentBall) * config.ballHandling.similarityAgentBallW * similarityAgentBall);
	}
	//Close to ball ? I assume you can reward for ball touch and similarity (similarityGoalBall)
	else {
		//In the zone to trigger
		if (!IsOutsideBackboardZone(player.carState, config.ballHandling.maxDistFromBB, config.ballHandling.minHeightToTrigger)) {
			//Touching the ball is a yes
			AddLog(reward, "Ball touch", player.ballTouchedStep * config.ballHandling.touchW);

			//Going towards the net is also a yes, make sure to be above the required threshold
			AddLog(reward, "Ball towards goal", (similarityGoalBall > config.goalHandling.minSimilarityGoalBall) * config.goalHandling.similarityGoalBallW);
		}
		
	}

	//Goal ? Count that in, add a lil' bonus for the speed
	const PlayerData* lastHit = this->physHandling.GetLatestHit(player.team);
	if (lastHit != nullptr) {
		if (state.scoreLine[(int)player.team] != scoreLine[(int)player.team] and !IsOutsideBackboardZone(lastHit->GetPhys(false), this->config.ballHandling.maxDistFromBB, this->config.ballHandling.minHeightToTrigger)) {

			//Of course, goal is gold
			AddLog(reward, "Goal", config.goalHandling.goalW);

			//Goal speed too, making sure to count that as a bonus rather than a goal farmer
			AddLog(reward, "Goal speed", state.ball.vel.Length() / RLGSC::CommonValues::BALL_MAX_SPEED * config.goalHandling.goalSpeedW);
		}
		else if (state.scoreLine[1 - (int)player.team] != scoreLine[1 - (int)player.team]) {
			AddLog(reward, "Concede", config.goalHandling.concedeW);
		}
	}
	
	scoreLine = state.scoreLine;

	return reward;
}

void UseDTReward::ClearChanges()
{
	this->dtReward.ClearChanges();
}

void UseDTReward::Reset(const GameState& initialState)
{
	LoggableReward::Reset(initialState);
}

void UseDTReward::Log(RLGPC::Report& report, std::string name, float weight)
{
	LoggableReward::Log(report, name, weight);
	this->dtReward.Log(report, name + "/DoubleTap", weight);
}

void DoubleTapReward::PhysObjHandling::Clear()
{
	this->allCurrentHits.clear();
}

void DoubleTapReward::PhysObjHandling::Reset(const GameState& initialState)
{
	this->allCurrentHits.clear();
	for (int i = 0; i < initialState.players.size(); i++) {
		PlayerData p = initialState.players[i];

		this->allCurrentHits[p.carId] = { .physObj = p, .lastStepHit = 0 };
	}
}

void DoubleTapReward::PhysObjHandling::Update(const GameState& const initialState, const PlayerData& player, const int nbSteps)
{
	if(player.ballTouchedStep) this->allCurrentHits[player.carId] = { .physObj = player, .lastStepHit = nbSteps };
}

const PlayerData* DoubleTapReward::PhysObjHandling::GetLatestHit(const Team team)
{
	const PlayerData* closest = nullptr;


	for (auto const& [key, val] : this->allCurrentHits)
	{
		if (val.physObj.team == team) closest = &val.physObj;
	}

	return closest;
}
