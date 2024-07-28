/*****************************************************************//**
 * \file   ReplaySetter.h
 * \brief  Setter that lets you reset on replay situations
 * 
 * \author Mathieu Suchet
 * \date   July 2024
 *********************************************************************/

#pragma once

#include <RLGymSim_CPP/Utils/StateSetters/StateSetter.h>
#include <Replays/ReplayFrame.h>
#include <Replays/ReplayUtils.h>

using namespace RLGSC;

START_REPLAY_NS

#define REPLAY_SETTER_LOG(content) VOID_LOG("[REPLAY SETTER] " << content)
#define REPLAY_SETTER_WARN(content) VOID_WARN("[REPLAY SETTER] " << content)
#define REPLAY_SETTER_ERR(content) VOID_ERR("[REPLAY SETTER] " << content)


/// <summary>
/// Setter that lets you reset on replay situations
/// </summary>
class ReplaySetter: public StateSetter {
public:
	
	/// <summary>
	/// A bool/string/int tuple to represent a toggle, a path and an end delay
	/// </summary>
	struct ReplayArgs {
		bool toggle;
	 	std::vector<std::string> paths;
		int endDelay = 0;
	};

	struct ReplaySetterArgs {

		/// <summary>
		/// If set to true, accepts and loads a json file containing replays
		/// </summary>
		ReplayArgs loadExistingReplays;

		/// <summary>
		/// If set to true, accepts and loads a folder of .replay files and converts them to replays (and saves them)
		/// </summary>
		ReplayArgs loadNewReplays;


		/// <summary>
		/// All the replays will be saved there
		/// </summary>
		std::string outputPath;

		/// <summary>
		/// Saves the replay to the outputPath
		/// </summary>
		bool saveReplays = false;
	};

	std::vector<Replay> replays;
	std::map<std::pair<int, int>, std::vector<Replay>> sortedReplays = {};
	ReplaySetter(ReplaySetterArgs args);

	virtual GameState ResetState(Arena* arena);
};
END_REPLAY_NS
