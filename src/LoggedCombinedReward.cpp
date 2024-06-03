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
		try {
			((LoggableReward*)rewardFuncs[i])->Log(report, names[i], rewardWeights[i]);
		}
		catch (std::exception e) {
			std::cout << e.what() << std::endl;
		}
		report.AccumAvg(REWARD_HEADER + std::get<0>(lastRewards[i]), std::get<1>(lastRewards[i]));
	}
}

void LoggableReward::Log(RLGPC::Report& report, std::string name, float weight)
{
	for (std::pair change : changes) {
		report.AccumAvg(REWARD_HEADER + name + "/" + change.first, change.second.size() > 0 ? std::accumulate(change.second.begin(), change.second.end(), 0.0f) / change.second.size() * weight : 0.0f);
	}
}

void LoggableReward::AddChange(float& value, std::string name, float change, bool empty)
{
	value += change;
	for (std::pair<std::string, RLGSC::FList>& changeDone : changes)
	{
		if (changeDone.first == name) {
			if (empty) {
				changeDone.second = {};
			}
			else {
				
				changeDone.second.push_back(change);
			}
			return;
		}
	}

	if (empty) {
		changes.push_back({ name, {} });
	}
	else {
		changes.push_back({ name, {change} });
	}
	
}

void LoggableReward::ClearChanges()
{
	changes.clear();
}

RLGSC::FList LoggableReward::GetAllRewards(const RLGSC::GameState& state, const RLGSC::ActionSet& prevActions, bool final)
{
	ClearChanges();
	return RewardFunction::GetAllRewards(state, prevActions, final);
}
