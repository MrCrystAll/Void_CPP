#pragma once

#include <Logging/LoggableReward.h>

USE_LOGGING_NS;


void LoggableReward::LogAll(RLGPC::Report& report, bool final, std::string name, float weight)
{
	final ? LogFinal(report, name, weight) : Log(report, name, weight);
}

void LoggableReward::Reset(const GameState& initialState)
{
	this->reward.ResetAll();
}

void LoggableReward::PreStep(const GameState& state)
{
	this->reward.Reset();
}

float LoggableReward::ComputeReward()
{
	float re = this->reward.value;
	this->reward.Reset();
	this->reward.Step();
	return re;
}

void LoggableReward::PrintReward(float weight, bool showMedian, bool showStd, bool showMin, bool showMax)
{
	VOID_LOG(" - " << this->name << ": " << this->reward.logs["_total"].ComputeAvg() * weight);
	for (auto& [key, val] : this->reward.logs) {
		if (key == "_total") continue;

		std::string medianT = showMedian ? "(median: " : "";
		std::string stdT = showMedian and showStd ? ", std: " : not showMedian and showStd ? "(std: " : "";
		std::string minT = (showMedian or showStd) and showMin ? ", min: " : not showMedian and not showStd and showMin ? "(min: " : "";
		std::string maxT = (showMedian or showStd or showMin) and showMax ? ", max: " : not showMedian and not showStd and not showMin and showMax ? "(max: " : "";


		VOID_LOG("\t - " << key << ": " << val.ComputeAvg() * weight << " " << (showMedian ? medianT + std::to_string(val.ComputeMedian() * weight) : "") << (showStd ? stdT + std::to_string(val.ComputeStd() * weight) : "") << (showMin ? minT + std::to_string(val.ComputeMin() * weight) : "") << (showMax ? maxT + std::to_string(val.ComputeMax() * weight) : "") << ")");
	}
}

void LoggableReward::LogFinal(RLGPC::Report& report, std::string name, float weight)
{
	//Compute average throughout episode
	for (auto& [key, val] : this->reward.logs) {
		std::string keyCopy;
		if (key == "_total") {
			keyCopy = "";
		}
		else {
			keyCopy = key;
		}

		//Sub reward logging doesn't log the sub reward total
		if ((not name.empty() and keyCopy.empty()) or keyCopy == "__temp") continue;

		if (name.empty()) {
			report.AccumAvg(REWARD_HEADER + this->name + (keyCopy.empty() ? "" : "/" + keyCopy), val.ComputeAvg() * weight);
		}
		else {
			report.AccumAvg(REWARD_HEADER + name + "/" + this->name + (keyCopy.empty() ? "" : "/" + keyCopy), val.ComputeAvg() * weight);
		}
	}
}

void LoggableReward::Log(Report& report, std::string name, float weight)
{
	//Do we log each step ? Don't think that's necessary, i'll leave it here in case
}

void LoggableWrapper::Reset(const GameState& initialState)
{
	LoggableReward::Reset(initialState);
	this->rfn->Reset(initialState);
}

void LoggableWrapper::PreStep(const GameState& state)
{
	LoggableReward::PreStep(state);
	this->rfn->PreStep(state);
}

float LoggableWrapper::GetReward(const PlayerData& player, const GameState& state, const Action& prevAction)
{
	this->reward += {this->rfn->GetReward(player, state, prevAction), "__temp"};
	return this->ComputeReward();
}

float LoggableWrapper::GetFinalReward(const PlayerData& player, const GameState& state, const Action& prevAction)
{
	this->reward += {this->rfn->GetFinalReward(player, state, prevAction), "__temp"};
	return this->ComputeReward();
}

#pragma region ZeroSum

void ZeroSumLoggedWrapper::Reset(const GameState& initialState)
{
	LoggableReward::Reset(initialState);
	this->rfn->Reset(initialState);
}

void ZeroSumLoggedWrapper::PreStep(const GameState& state)
{
	LoggableReward::PreStep(state);
	this->rfn->PreStep(state);
}

float ZeroSumLoggedWrapper::GetReward(const PlayerData& player, const GameState& state, const Action& prevAction)
{
	return this->rfn->GetReward(player, state, prevAction);
}

float ZeroSumLoggedWrapper::GetFinalReward(const PlayerData& player, const GameState& state, const Action& prevAction)
{
	return this->rfn->GetFinalReward(player, state, prevAction);
}

std::vector<float> ZeroSumLoggedWrapper::GetAllRewards(const GameState& state, const ActionSet& prevActions, bool final)
{
	std::vector<float> rewards = LoggableReward::GetAllRewards(state, prevActions, final);

	int teamCounts[2] = {};
	float avgTeamRewards[2] = {};

	for (int i = 0; i < state.players.size(); i++) {
		int teamIdx = (int)state.players[i].team;
		teamCounts[teamIdx]++;
		avgTeamRewards[teamIdx] += rewards[i];
	}

	for (int i = 0; i < 2; i++)
		avgTeamRewards[i] /= RS_MAX(teamCounts[i], 1);

	for (int i = 0; i < state.players.size(); i++) {
		auto& player = state.players[i];
		int teamIdx = (int)player.team;
		int teamCount = teamCounts[teamIdx];

		this->reward.value = rewards[i];
		this->reward *= {1 - this->teamSpirit, "Zero sum | Team spirit distribution"};
		this->reward += {avgTeamRewards[teamIdx] * this->teamSpirit, "Zero sum | Team spirit"};
		this->reward -= {avgTeamRewards[1 - teamIdx] * this->oppScaling, "Zero sum | Opponent scaling"};

		this->reward.Reset();

		rewards[i] =
			rewards[i] * (1 - this->teamSpirit)
			+ (avgTeamRewards[teamIdx] * this->teamSpirit)
			- (avgTeamRewards[1 - teamIdx] * this->oppScaling);
	}

	return rewards;
}

#pragma endregion