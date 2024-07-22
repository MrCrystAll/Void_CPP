#include "Rewards/DoubleTap/DoubleTapReward.h"

USE_REWARDS_DT_NS;

void DoubleTapReward::Reset(const RLGSC::GameState& initialState)
{
	this->physHandling.Reset(initialState);
	this->nbSteps = 0;
	this->scoreLine = initialState.scoreLine;
	LoggableReward::Reset(initialState);
}
RLGSC::FList DoubleTapReward::GetAllRewards(const RLGSC::GameState& state, const RLGSC::ActionSet& prevActions, bool final)
{
	this->nbSteps++;
	return LoggableReward::GetAllRewards(state, prevActions, final);
}

float DoubleTapReward::GetReward(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction)
{
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

		reward += {-agentToBallDist / config.ballHandling.distBallReduction, "Distance to ball"};
		reward += {(similarityAgentBall > config.ballHandling.minSimilarityAgentBall) * config.ballHandling.similarityAgentBallW* similarityAgentBall, "Going towards the ball"};
	}
	//Close to ball ? I assume you can reward for ball touch and similarity (similarityGoalBall)
	else {
		//In the zone to trigger
		if (!IsOutsideBackboardZone(player.carState, config.ballHandling.maxDistFromBB, config.ballHandling.minHeightToTrigger)) {
			//Touching the ball is a yes
			reward += {player.ballTouchedStep* config.ballHandling.touchW, "Ball touch"};


			//Going towards the net is also a yes, make sure to be above the required threshold
			reward += {(similarityGoalBall > config.goalHandling.minSimilarityGoalBall)* config.goalHandling.similarityGoalBallW, "Ball towards goal"};

		}
		
	}

	//Goal ? Count that in, add a lil' bonus for the speed
	const PlayerData* lastHit = this->physHandling.GetLatestHit(player.team);
	if (lastHit != nullptr) {
		if (state.scoreLine[(int)player.team] != scoreLine[(int)player.team] and !IsOutsideBackboardZone(lastHit->GetPhys(false), this->config.ballHandling.maxDistFromBB, this->config.ballHandling.minHeightToTrigger)) {

			//Of course, goal is gold
			reward += {config.goalHandling.goalW, "Goal"};


			//Goal speed too, making sure to count that as a bonus rather than a goal farmer
			reward += { state.ball.vel.Length() / RLGSC::CommonValues::BALL_MAX_SPEED * config.goalHandling.goalSpeedW, "Goal speed"};

		}
		else if (state.scoreLine[1 - (int)player.team] != scoreLine[1 - (int)player.team]) {
			reward += {config.goalHandling.concedeW, "Concede"};

		}
	}
	
	scoreLine = state.scoreLine;
	return this->ComputeReward();
}

void UseDTReward::Reset(const RLGSC::GameState& initialState)
{
	LoggableReward::Reset(initialState);
	this->dtReward.Reset(initialState);
}

std::vector<float> UseDTReward::GetAllRewards(const GameState& state, const ActionSet& prevActions, bool final)
{
	std::vector<float> rewards = std::vector<float>(state.players.size());
	for (int i = 0; i < state.players.size(); i++) {
		if (final) {
			rewards[i] = GetFinalReward(state.players[i], state, prevActions[i]);
		}
		else {
			rewards[i] = GetReward(state.players[i], state, prevActions[i]);
		}
		this->reward.Reset();
		this->dtReward.reward.Reset();
		this->reward.Step();
		this->dtReward.reward.Step();
	}
	return rewards;
}

void UseDTReward::LogAll(RLGPC::Report& report, bool final, std::string name, float weight)
{
	LoggableReward::LogAll(report, final, name, weight);
	this->dtReward.LogAll(report, final, this->name, weight);
}

void DoubleTapReward::PhysObjHandling::Clear()
{
	this->allCurrentHits.clear();
}

void DoubleTapReward::PhysObjHandling::Reset(const RLGSC::GameState& initialState)
{
	this->allCurrentHits.clear();
	for (int i = 0; i < initialState.players.size(); i++) {
		PlayerData p = initialState.players[i];

		this->allCurrentHits[p.carId] = { .physObj = p, .lastStepHit = 0 };
	}
}

void DoubleTapReward::PhysObjHandling::Update(const RLGSC::GameState& initialState, const RLGSC::PlayerData& player, const int nbSteps)
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



void UseDTReward::PreStep(const GameState& state) {
	this->dtReward.PreStep(state);
	LoggableReward::PreStep(state);
}