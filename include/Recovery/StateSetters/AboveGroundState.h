#pragma once
#include <RLGymSim_CPP/Utils/StateSetters/StateSetter.h>
#include "RLGymSim_CPP/Utils/StateSetters/RandomState.h"

#include <Recovery/RecoveryUtils.h>

START_RECOVERY_STATES_NS

using namespace RLGSC;
class OverfittingGroundRecoveryState : public RLGSC::StateSetter{
	// Inherited via StateSetter
	GameState ResetState(Arena* arena) override;
};

class RandomRecoveryState : public RLGSC::StateSetter {
	// Inherited via StateSetter
	GameState ResetState(Arena* arena) override;

	RandomState randomState = RandomState(true, true, true);
};

END_RECOVERY_NS
