#include "TerminalConditions.h"

USE_TC_NS;

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

BounceTimeoutCondition::BounceTimeoutCondition(int steps_after_bounce): totalSteps(steps_after_bounce)
{
}

void BounceTimeoutCondition::Reset(const RLGSC::GameState& initialState)
{
	this->currentSteps = 0;
}

bool BounceTimeoutCondition::IsTerminal(const RLGSC::GameState& currentState)
{
	if(this->currentSteps > 0 or currentState.ball.pos.z < RLGSC::CommonValues::BALL_RADIUS + 50){
		this->currentSteps++;
	}

	return this->currentSteps >= this->totalSteps;
}

bool TouchTimeoutCondition::IsTerminal(const RLGSC::GameState& currentState)
{
	for (RLGSC::PlayerData p : currentState.players) {
		if (p.ballTouchedStep) return true;
	}
	return false;
}
