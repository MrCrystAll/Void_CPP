#include "LoggedCombinedReward.h"
#include "LoggerUtils.h"

LoggedCombinedReward::LoggedCombinedReward(std::vector<RewardFunction*> rewardFuncs, std::vector<float> rewardWeights, std::vector<std::string> names, bool ownsFuncs) :rewardFuncs(rewardFuncs), rewardWeights(rewardWeights), names(names), ownsFuncs(ownsFuncs), lastRewards() {
	assert(rewardFuncs.size() == rewardWeights.size());
}

LoggedCombinedReward::LoggedCombinedReward(std::vector<std::tuple<RewardFunction*, float, std::string>> funcsWithWeights, bool ownsFuncs) :
	ownsFuncs(ownsFuncs), lastRewards() {
	for (auto& pair : funcsWithWeights) {
		rewardFuncs.push_back(std::get<0>(pair));
		rewardWeights.push_back(std::get<1>(pair));
		names.push_back(std::get<2>(pair));
	}
}

void LoggedCombinedReward::LogRewards(RLGPC::Report& report) {
	for (int i = 0; i < lastRewards.size(); i++) {
		report.AccumAvg(REWARD_HEADER + std::get<0>(lastRewards[i]), std::get<1>(lastRewards[i]));
	}
}