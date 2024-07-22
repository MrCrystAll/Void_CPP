#pragma once

#include <RLGymSim_CPP/Utils/StateSetters/StateSetter.h>
#include <Replays/ReplayFrame.h>
#include <Replays/ReplayUtils.h>

using namespace RLGSC;

START_REPLAY_NS
class ReplaySetter: public StateSetter {
public:
	std::vector<Replay> replays;
	ReplaySetter(std::string replaysToLoad);

	virtual GameState ResetState(Arena* arena);
private:
	void ReplayFrameToState(ReplayFrame frame, Arena* arena);
};
END_REPLAY_NS
