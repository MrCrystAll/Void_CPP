#include "Utils/Selector/Submodel.h"

USE_SELECTOR_NS;

RLGSC::ActionSet Submodel::Act(const RLGSC::GameState& state, const RLGSC::ActionSet& prevActions, bool deterministic)
{
	return this->agent.InferPolicyAll(state, prevActions, deterministic);
}
