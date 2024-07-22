#pragma once

#include <Replays/CarballHandling.h>
#include <Replays/ReplayFrame.h>
#include <Replays/ReplayUtils.h>
#include "../../RLGymPPO_CPP/libsrc/json/nlohmann/json.hpp"
#include <istream>

using json = nlohmann::json;

START_REPLAY_NS

class ReplayLoader {
public:
	/// <summary>
	/// Loads a replay given a file
	/// </summary>
	/// <param name="path">The path to the replay</param>
	/// <param name="endDelay">The number of frames to ignore before a goal is scored</param>
	/// <returns></returns>
	Replay LoadReplay(std::string path, int endDelay);

private:
	std::vector<GameFrame> LoadGameFrames(std::string path, ReplayAnalysis analysis);
	std::vector<BallFrame> LoadBallFrames(std::string path, ReplayAnalysis analysis);
	std::vector<std::vector<PlayerFrame>> LoadPlayersFrames(std::string path, ReplayAnalysis analysis);
	ReplayMetadata LoadMetadata(std::string path, ReplayAnalysis analysis);
	ReplayAnalysis LoadAnalysis(std::string path, int endDelay);

};
END_REPLAY_NS