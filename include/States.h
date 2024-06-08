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

class WallPinchSetter : public RLGSC::StateSetter {
public:
	WallPinchSetter(float posVariance = 400.0f, float velVariance = 200.0f, float orientVariance = 0.5f);
	virtual RLGSC::GameState ResetState(Arena* arena);

private:
	float posVariance;
	float orientVariance;
	float velVariance;
};


class CeilingPinchSetter : public RLGSC::StateSetter {
public:
	CeilingPinchSetter(float posVariance = 100.0f, float velVariance = 200.0f, float orientVariance = 0.2f);
	virtual RLGSC::GameState ResetState(Arena* arena);

private:
	float posVariance;
	float orientVariance;
	float velVariance;
};

class OverfittingCeilingPinchSetter : public RLGSC::StateSetter {
public:
	virtual RLGSC::GameState ResetState(Arena* arena);
};