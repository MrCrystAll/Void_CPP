#include "TerminalConditions.h"

TimeoutCondition::TimeoutCondition(int steps_before_to) : totalSteps(steps_before_to), currentSteps(0)
{
	
}

void TimeoutCondition::Reset(const RLGSC::GameState& initialState)
{
	this->currentSteps = 0;
}

bool TimeoutCondition::IsTerminal(const RLGSC::GameState& currentState)
{
	this->currentSteps++;
	return this->currentSteps >= this->totalSteps;
}
