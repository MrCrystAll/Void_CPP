/*****************************************************************//**
 * \file   ReplayFilter.h
 * \brief  The object used to use the rcfs
 * 
 * \author Mathieu Suchet
 * \date   July 2024
 *********************************************************************/

#pragma once

#include <Replays/RCF/AbstractRCF.h>
#include <Replays/ReplayFrame.h>

START_RCF_NS

/// <summary>
/// The object used to use the rcfs
/// </summary>
class ReplayFilter {
private:
	AbstractRCF* rcf;
public:
	ReplayFilter(AbstractRCF* rcf) : rcf(rcf) {};

	/// <summary>
	/// Filters a replay using its rcf
	/// </summary>
	/// <param name="replay">The replay to filter</param>
	/// <returns>Filtered replay</returns>
	Replay FilterReplay(Replay replay);

	/// <summary>
	/// Filters replays using its rcf
	/// </summary>
	/// <param name="replays">The replays to filter</param>
	/// <returns>Filtered replays</returns>
	std::vector<Replay> FilterReplays(std::vector<Replay> replays);
};

END_RCF_NS