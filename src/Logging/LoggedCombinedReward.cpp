#include "Logging/LoggedCombinedReward.h"
#include <RLGymPPO_CPP/Util/Report.h>
#include <Logging/LoggableReward.h>

using namespace RLGPC;

void LoggedCombinedReward::LogAll(RLGPC::Report& report, bool final, std::string name, float weight)
{
	for (size_t i = 0; i < this->rewardsAndWeights.size(); i++)
	{
		std::pair<RewardFunction*, float> pair = this->rewardsAndWeights[i];
		LoggableReward* lrw = dynamic_cast<LoggableReward*>(pair.first);
		if (lrw != nullptr) {
			lrw->LogAll(report, final, "", pair.second);
		}
	}
}

void LoggedCombinedReward::Reset(const GameState& initialState) {
	for (size_t i = 0; i < this->rewardsAndWeights.size(); i++)
	{
		std::pair<RewardFunction*, float> pair = this->rewardsAndWeights[i];
		pair.first->Reset(initialState);
	}

}

void LoggedCombinedReward::PreStep(const GameState& state) {
	for (size_t i = 0; i < this->rewardsAndWeights.size(); i++)
	{
		std::pair<RewardFunction*, float> pair = this->rewardsAndWeights[i];
		pair.first->PreStep(state);
	}
}

float LoggedCombinedReward::GetReward(const PlayerData& player, const GameState& state, const Action& prevAction) {
	float r = 0;
	for (size_t i = 0; i < this->rewardsAndWeights.size(); i++)
	{
		std::pair<RewardFunction*, float> pair = this->rewardsAndWeights[i];
		float result = pair.first->GetReward(player, state, prevAction);
		r += result * pair.second;
	}

	return r;
}

float LoggedCombinedReward::GetFinalReward(const PlayerData& player, const GameState& state, const Action& prevAction) {
	float r = 0;

	for (size_t i = 0; i < this->rewardsAndWeights.size(); i++)
	{
		std::pair<RewardFunction*, float> pair = this->rewardsAndWeights[i];
		float result = pair.first->GetFinalReward(player, state, prevAction);
		r += result * pair.second;
	}

	return r;
}

// Get all rewards for all players
std::vector<float> LoggedCombinedReward::GetAllRewards(const GameState& state, const ActionSet& prevActions, bool final) {
	RLGPC::FList rewards = FList(state.players.size(), 0.0f);
	for (size_t i = 0; i < this->rewardsAndWeights.size(); i++)
	{
		std::pair<RewardFunction*, float> pair = this->rewardsAndWeights[i];
		FList result = pair.first->GetAllRewards(state, prevActions, final);
		for (int j = 0; j < result.size(); j++) {
			rewards[j] += result[j] * pair.second;
		}
	}

	return rewards;
};

void LoggedCombinedReward::PrintRewards(bool showMedian, bool showStd, bool showMin, bool showMax) {
	VOID_LOG(" =========== Episode summary ============= ");
	for (std::pair<RewardFunction*, float> var : this->rewardsAndWeights)
	{
		LoggableReward* lgr = dynamic_cast<LoggableReward*>(var.first);
		if (lgr != nullptr)
		{
			lgr->PrintReward(var.second, showMedian, showStd, showMin, showMax);
		}
		else {
			VOID_WARN("Couldn't log reward");
		}
	}

	VOID_LOG(" ========================================= ");
}