#include <Replays/RCF/ReplayFilter.h>

USE_RCF_NS;
USE_REPLAY_NS;

Replay ReplayFilter::FilterReplay(Replay replay)
{
	Replay finalReplay = Replay();

	//Goal frames will be off too
	finalReplay.metadata = replay.metadata;

	//The analysis will be off, so we don't replicate it

	//Actual filtering
	std::vector<GameState> filteredStates = this->rcf->MatchStates(replay.states);
	VOID_LOG("Filtering done, removed " << replay.states.size() - filteredStates.size() << " states (" << filteredStates.size() << " states left)");
	finalReplay.states = filteredStates;

	//I'm modifying this so the replay setter doesn't die on out of bounds
	finalReplay.metadata.numberOfPlayableFrames = filteredStates.size();

	return finalReplay;
}

std::vector<Replay> ReplayFilter::FilterReplays(std::vector<Replay> replays)
{
	std::vector<Replay> finalReplays;

	for (Replay r : replays) {
		finalReplays.push_back(this->FilterReplay(r));
	}

	return finalReplays;
}
