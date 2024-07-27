#include <Replays/RCF/AbstractRCF.h>

USE_RCF_NS;

bool AbstractRCF::AllPlayersFilter(const GameState state)
{
    for (const PlayerData p : state.players) {
        if(this->PlayerFilter(p, state)) return true;
    }
    return false;
}

std::vector<GameState> AbstractRCF::MatchStates(const std::vector<GameState>& states)
{
    std::vector<GameState> matchedStates = {};
    for (const GameState gs : states) {
        if (this->AllPlayersFilter(gs)) matchedStates.push_back(gs);
    }

    return matchedStates;
}

bool AbstractQueuedRCF::AllPlayersFilter(const GameState state)
{
    bool result = AbstractRCF::AllPlayersFilter(state);
    this->states.PushToQueue(state);
    return result;
}

std::vector<GameState> AbstractQueuedRCF::MatchStates(const std::vector<GameState>& states)
{
    std::vector<GameState> matchedStates = {};
    for (const GameState gs : states) {
        if (this->AllPlayersFilter(gs)) matchedStates.push_back(this->states.GetFront());
    }

    return matchedStates;
}
