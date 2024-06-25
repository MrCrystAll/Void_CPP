#pragma once

#include <RLGymSim_CPP/Utils/StateSetters/StateSetter.h>
#include <RLGymPPO_CPP/Lists.h>

class WeightedSampleSetter : public RLGSC::StateSetter {
private:
	std::vector<std::pair<StateSetter*, float>> setters;

	//Currently ignored
	RLGPC::FList gmProbs;
	virtual void BuildArenaConfig(Arena* arena);

public:
	WeightedSampleSetter(std::vector<std::pair<StateSetter*, float>> setters, RLGPC::FList gmProbs) : setters(setters), gmProbs(gmProbs) {};
	virtual RLGSC::GameState ResetState(Arena* arena) override;
};