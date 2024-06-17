#include "ObsBuilder.h"

RLGSC::FList OnesObs::BuildOBS(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction)
{
    RLGSC::FList result = {};
    for (int i = 0; i < 70; i++) {
        result.push_back(1);
    }

    return result;
};
