#include <TerminalConditions.h>

START_TC_NS

bool NTouchTimeoutCondition::IsTerminal(const RLGSC::GameState& currentState)
{
    for (const RLGSC::PlayerData& p : currentState.players) {
        this->currentNTouches[p.carId] += p.ballTouchedStep;
        if (this->currentNTouches[p.carId] >= this->nTouches) {
            return true;
        }
    }
    return false;
}

void NTouchTimeoutCondition::Reset(const RLGSC::GameState& initialState)
{
    this->currentNTouches.clear();
    for (const RLGSC::PlayerData& p : initialState.players) {
        this->currentNTouches[p.carId] = 0;
    }
}

END_TC_NS
