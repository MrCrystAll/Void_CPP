#pragma once

#include <Replays/RCF/AbstractRCF.h>
#include <Replays/ReplayFrame.h>

START_RCF_NS

class ReplayFilter {
private:
	AbstractRCF* rcf;
public:
	ReplayFilter(AbstractRCF* rcf) : rcf(rcf) {};

	Replay FilterReplay(Replay replay);
	std::vector<Replay> FilterReplays(std::vector<Replay> replays);
};

END_RCF_NS