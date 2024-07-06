#pragma once

#include <Logging/LoggableReward.h>


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

		//Sub reward logging doesn't log the subreward total
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
std::vector<float> LoggableReward::GetAllRewards(const GameState& state, const ActionSet& prevActions, bool final)
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
		this->reward.Step();
	}
	return rewards;
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
	return this->reward.value;
}

float LoggableWrapper::GetFinalReward(const PlayerData& player, const GameState& state, const Action& prevAction)
{
	this->reward += {this->rfn->GetFinalReward(player, state, prevAction), "__temp"};
	return this->reward.value;
}
