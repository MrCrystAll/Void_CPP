/*****************************************************************//**
 * \file   RCFMain.cpp
 * \brief  Use this to use the rcfs and have filtered replays
 * 
 * \author msuch
 * \date   July 2024
 *********************************************************************/

// RCF
#include <Replays/RCF/AbstractRCF.h>
#include <Replays/RCF/CommonRCFs.h>
#include <Replays/RCF/MultiRCF.h>
#include <Replays/RCF/RCFUtils.h>

//Filtering
#include <Replays/RCF/ReplayFilter.h>

//Replay loading/saving
#include <Replays/ReplayLoader.h>
#include <Replays/ReplaySaver.h>

//Replays
#include <Replays/ReplayFrame.h>

USE_REPLAY_NS;
USE_RCF_NS;

int main() {
	std::string INPUT_PATH = "replays/3s"; //Can be replay or json
	std::string OUTPUT_PATH = "FlipResets3v3.json"; //Has to be json
	bool IS_JSON = INPUT_PATH.ends_with(".json");

	bool isDirectory = std::filesystem::is_directory(INPUT_PATH);

	//RCF and filter
	AbstractRCF* rcf = new FlipResetRCF();

	ReplayFilter filter = ReplayFilter(rcf);
	ReplaySaver saver = ReplaySaver();

	if (not OUTPUT_PATH.ends_with(".json")) {
		VOID_ERR("OUTPUT_PATH must end with .json, current value: " << OUTPUT_PATH);
		exit(EXIT_FAILURE);
	}

	if (IS_JSON) {
		if (isDirectory) {
			//You can do it yourself here
			VOID_ERR("Loading JSONs from a directory is not supported yet (you can do it yourself in the file)");
			exit(EXIT_FAILURE);
		}
		else {
			std::ifstream replayFile = std::ifstream(INPUT_PATH);
			json j;
			VOID_LOG("Loading replays from " << INPUT_PATH << "...");
			replayFile >> j;

			std::vector<Replay> replays = j;
			VOID_LOG("Loaded " << replays.size() << " replays");

			VOID_LOG("Fitering " << replays.size() << " replays...");
			std::vector<Replay> filteredReplays = filter.FilterReplays(replays);
			VOID_LOG("Finished filtering");

			saver.SaveReplays(OUTPUT_PATH, filteredReplays, false);

			replayFile.close();
		}
	}
	else {
		//Need that for interpolation
		RocketSim::Init("./collision_meshes");

		int endDelay = 50;

		ReplayLoader loader = ReplayLoader();

		std::vector<Replay> replays;
		if (isDirectory) {
			replays = loader.LoadReplays(INPUT_PATH, endDelay);
		}
		else {
			Replay r = loader.LoadReplay(INPUT_PATH, endDelay);
			replays = { r };
		}

		std::vector<Replay> filteredReplays = filter.FilterReplays(replays);
		saver.SaveReplays(OUTPUT_PATH, filteredReplays, true);
	}
}
