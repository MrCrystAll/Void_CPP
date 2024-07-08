/*****************************************************************//**
 * \file   States.h
 * \brief  Weighted sample setter
 * 
 * \author Mathieu Suchet
 * \date   July 2024
 *********************************************************************/

#pragma once

#include <RLGymSim_CPP/Utils/StateSetters/StateSetter.h>
#include <RLGymPPO_CPP/Lists.h>


/**
 * @brief Setter that allows to pick a random state in a list of weighted states
 */
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

class EmptyBoostKickoffSetter : public RLGSC::StateSetter {
public:
	virtual RLGSC::GameState ResetState(Arena* arena) override;
};
