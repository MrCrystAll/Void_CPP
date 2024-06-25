#pragma once

#include "RLGymSim_CPP/Utils/StateSetters/StateSetter.h"
#include "Utils/PinchUtils.h"

START_PINCH_NS

class WallPinchSetter : public RLGSC::StateSetter {
public:
	WallPinchSetter(float posVariance = 400.0f, float velVariance = 200.0f, float orientVariance = 0.5f);
	virtual RLGSC::GameState ResetState(Arena* arena);

private:
	float posVariance;
	float orientVariance;
	float velVariance;
};

END_PINCH_NS