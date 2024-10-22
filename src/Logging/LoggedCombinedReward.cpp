#include "Logging/LoggedCombinedReward.h"
#include <RLGymPPO_CPP/Util/Report.h>
#include <Logging/LoggableReward.h>

USE_LOGGING_NS;

using namespace RLGPC;

void LoggedCombinedReward::LogAll(RLGPC::Report& report, bool final, std::string name, float weight)
{
	for (size_t i = 0; i < this->rewardsAndWeights.size(); i++)
	{
		RewardProp pair = this->rewardsAndWeights[i];
		pair.rf->LogAll(report, final, "", pair.w);
	}
}

void LoggedCombinedReward::Reset(const GameState& initialState) {
	LoggableReward::Reset(initialState);

	for (size_t i = 0; i < this->rewardsAndWeights.size(); i++)
	{
		RewardProp pair = this->rewardsAndWeights[i];
		pair.rf->Reset(initialState);
	}

}

void LoggedCombinedReward::PreStep(const GameState& state) {
	LoggableReward::PreStep(state);
	for (size_t i = 0; i < this->rewardsAndWeights.size(); i++)
	{
		RewardProp pair = this->rewardsAndWeights[i];
		pair.rf->PreStep(state);
	}
}

float LoggedCombinedReward::GetReward(const PlayerData& player, const GameState& state, const Action& prevAction) {
	for (size_t i = 0; i < this->rewardsAndWeights.size(); i++)
	{
		RewardProp pair = this->rewardsAndWeights[i];
		float result = pair.rf->GetReward(player, state, prevAction);
		this->reward += {result * pair.w, pair.name};

		pair.rf->reward.Step();
	}

	return this->ComputeReward();
}

float LoggedCombinedReward::GetFinalReward(const PlayerData& player, const GameState& state, const Action& prevAction) {
	LoggableReward::GetFinalReward(player, state, prevAction);
	for (size_t i = 0; i < this->rewardsAndWeights.size(); i++)
	{
		RewardProp pair = this->rewardsAndWeights[i];
		float result = pair.rf->GetFinalReward(player, state, prevAction);
		this->reward += {result* pair.w, pair.name};

		pair.rf->reward.Step();
	}

	return this->ComputeReward();
}

// Get all rewards for all players
std::vector<float> LoggedCombinedReward::GetAllRewards(const GameState& state, const ActionSet& prevActions, bool final) {
	RLGPC::FList rewards = FList(state.players.size(), 0.0f);
	for (size_t i = 0; i < state.players.size(); i++)
	{
		rewards[i] = final ? this->GetFinalReward(state.players[i], state, prevActions[i]): this->GetReward(state.players[i], state, prevActions[i]);
		this->reward.Step();
	}


	return rewards;
};

void LoggedCombinedReward::PrintReward(float weight, bool showMedian, bool showStd, bool showMin, bool showMax) {
	LoggableReward::PrintReward(weight, showMedian, showStd, showMin, showMax);

	VOID_LOG("=============== Logged combined inner rewards ===============");

	for (RewardProp var : this->rewardsAndWeights)
	{
		var.rf->PrintReward(var.w * weight, showMedian, showStd, showMin, showMax);
	}

	VOID_LOG(" ========================================= ");
}