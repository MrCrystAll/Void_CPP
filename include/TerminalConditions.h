#pragma once
#include <RLGymSim_CPP/Utils/TerminalConditions/TerminalCondition.h>

class TimeoutCondition : public RLGSC::TerminalCondition {
public:
	TimeoutCondition(int steps_before_to);
	virtual void Reset(const RLGSC::GameState& initialState);
	virtual bool IsTerminal(const RLGSC::GameState& currentState);

private:
	int totalSteps;
	int currentSteps;
};

class BounceTimeoutCondition : public RLGSC::TerminalCondition {
public:
	BounceTimeoutCondition(int steps_after_bounce);
	virtual void Reset(const RLGSC::GameState& initialState);
	virtual bool IsTerminal(const RLGSC::GameState& currentState);
private:
	int totalSteps;
	int currentSteps = 0;
};