#pragma once

#include <Replays/CarballHandling.h>
#include <Replays/ReplayFrame.h>
#include <Replays/ReplayUtils.h>
#include <Replays/ReplaySaver.h>
#include "../../RLGymPPO_CPP/libsrc/json/nlohmann/json.hpp"
#include <istream>

using json = nlohmann::json;

START_REPLAY_NS

#define REPLAY_LOADER_LOG(content) VOID_LOG("[REPLAY LOADER] " << content)
#define REPLAY_LOADER_WARN(content) VOID_WARN("[REPLAY LOADER] " << content)
#define REPLAY_LOADER_ERR(content) VOID_ERR("[REPLAY LOADER] " << content)

class ReplayLoader {
public:
	/// <summary>
	/// Loads a replay given a file
	/// </summary>
	/// <param name="path">The path to the replay</param>
	/// <param name="endDelay">The number of frames to ignore before a goal is scored</param>
	/// <returns></returns>
	Replay LoadReplay(std::string path, int endDelay, bool saveReplay = false);

	std::vector<Replay> LoadReplays(std::string path, int delay, bool saveReplay = false);

private:
	ReplaySaver replaySaver = ReplaySaver();

	std::vector<GameFrame> LoadGameFrames(std::string path, ReplayAnalysis analysis);
	std::vector<BallFrame> LoadBallFrames(std::string path, ReplayAnalysis analysis);
	std::vector<std::vector<PlayerFrame>> LoadPlayersFrames(std::string path, ReplayAnalysis analysis);
	ReplayMetadata LoadMetadata(std::string path, ReplayAnalysis analysis);
	ReplayAnalysis LoadAnalysis(std::string path, int endDelay);
	std::vector<RLGSC::GameState> InterpolateReplays(ConvertedReplay replay);

};
END_REPLAY_NS