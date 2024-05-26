#pragma once
#include <RLGymSim_CPP/Utils/RewardFunctions/RewardFunction.h>
#include <numeric>
#include <RLGymPPO_CPP/Util/Report.h>

class LoggedCombinedReward : public RLGSC::RewardFunction {
public:
	std::vector<RewardFunction*> rewardFuncs;
	std::vector<float> rewardWeights;
	std::vector<std::tuple<std::string, float>> lastRewards;
	std::vector<std::string> names;
	bool ownsFuncs;

	LoggedCombinedReward(std::vector<RewardFunction*> rewardFuncs, std::vector<float> rewardWeights, std::vector<std::string> names, bool ownsFuncs = false);

	LoggedCombinedReward(std::vector<std::tuple<RewardFunction*, float, std::string>> funcsWithWeights, bool ownsFuncs = false);

	virtual void LogRewards(RLGPC::Report& report);

protected:
	virtual void Reset(const RLGSC::GameState& initialState) {
		for (auto func : rewardFuncs)
			func->Reset(initialState);
		
	}

	virtual void PreStep(const RLGSC::GameState& state) {
		for (auto func : rewardFuncs)
			func->PreStep(state);
	}

	virtual std::vector<float> GetAllRewards(const RLGSC::GameState& state, const RLGSC::ActionSet& prevAction, bool final) {
		std::vector<float> allRewards(state.players.size());
		lastRewards.clear();

		for (int i = 0; i < rewardFuncs.size(); i++) {
			auto rewards = rewardFuncs[i]->GetAllRewards(state, prevAction, final);
			float mean = std::accumulate(rewards.begin(), rewards.end(), 0.0) / rewards.size() * rewardWeights[i];
			this->lastRewards.push_back({this->names[i], mean});
			for (int j = 0; j < rewards.size(); j++) {
				allRewards[j] += rewards[j] * rewardWeights[i];
			}
		}

		return allRewards;
	}

	virtual ~LoggedCombinedReward() {
		if (ownsFuncs)
			for (auto func : rewardFuncs)
				delete func;
	}
};