/*****************************************************************//**
 * \file   CeilingPinchState.h
 * \brief  Overfitting and little random ceiling pinch state setters
 * 
 * \author Mathieu Suchet
 * \date   July 2024
 *********************************************************************/

#pragma once

#include "RLGymSim_CPP/Utils/StateSetters/StateSetter.h"
#include "Utils/PinchUtils.h"

START_PINCH_NS

class OverfittingCeilingPinchSetter : public RLGSC::StateSetter {
public:
	virtual RLGSC::GameState ResetState(Arena* arena);
};

class CeilingPinchSetter : public RLGSC::StateSetter {
public:
	CeilingPinchSetter(float posVariance = 400.0f, float velVariance = 200.0f, float orientVariance = 0.3f) : posVariance(posVariance), velVariance(velVariance), orientVariance(orientVariance) {};
	virtual RLGSC::GameState ResetState(Arena* arena);

private:
	float posVariance;
	float orientVariance;
	float velVariance;
};

END_PINCH_NS