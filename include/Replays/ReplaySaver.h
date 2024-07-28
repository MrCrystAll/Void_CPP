/*****************************************************************//**
 * \file   ReplaySaver.h
 * \brief  A class that allows you to save replays to json
 * 
 * \author Mathieu Suchet
 * \date   July 2024
 *********************************************************************/

#pragma once

#include <Replays/ReplayFrame.h>
#include <Replays/ReplayUtils.h>

START_REPLAY_NS

#define REPLAY_SAVER_LOG(content) VOID_LOG("[REPLAY SAVER] " << content)
#define REPLAY_SAVER_WARN(content) VOID_WARN("[REPLAY SAVER] " << content)
#define REPLAY_SAVER_ERR(content) VOID_ERR("[REPLAY SAVER] " << content)

/// <summary>
/// A class that allows you to save replays to json
/// </summary>
class ReplaySaver {
public:

	/// <summary>
	/// Serializes a replay to json
	/// </summary>
	/// <param name="replay">The replay to serialize</param>
	/// <returns>The serialized replay (json)</returns>
	json SerializeReplay(Replay replay);

	/// <summary>
	/// Saves a replay to a file
	/// </summary>
	/// <param name="path">Path to the file</param>
	/// <param name="replay">Replay to save</param>
	void SaveReplay(std::string path, Replay replay);

	/// <summary>
	/// Saves replays to a file
	/// </summary>
	/// <param name="path">Path to the file</param>
	/// <param name="replays">Replays to save</param>
	void SaveReplays(std::string path, std::vector<Replay> replays);
};

END_REPLAY_NS