#pragma once

#include <RLGymSim_CPP/Utils/StateSetters/StateSetter.h>

class WallPinchSetter : public RLGSC::StateSetter {
	virtual RLGSC::GameState ResetState(Arena* arena);
};