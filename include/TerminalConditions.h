/*****************************************************************//**
 * \file   TerminalConditions.h
 * \brief  Custom terminal conditions
 * 
 * \author Mathieu Suchet
 * \date   July 2024
 *********************************************************************/

#pragma once
#include <Utils/VoidUtils.h>
#include <RLGymSim_CPP/Utils/TerminalConditions/TerminalCondition.h>

START_TC_NS

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

class TouchTimeoutCondition : public RLGSC::TerminalCondition {
	// Inherited via TerminalCondition
	bool IsTerminal(const RLGSC::GameState& currentState) override;
};

class NTouchTimeoutCondition : public RLGSC::TerminalCondition {
public:
	NTouchTimeoutCondition(int nTouches) : nTouches(nTouches) {};

	// Inherited via TerminalCondition
	bool IsTerminal(const RLGSC::GameState& currentState) override;
	virtual void Reset(const RLGSC::GameState& initialState) override;

private:
	int nTouches = 0;
	std::map<int, int> currentNTouches;
};

END_TC_NS