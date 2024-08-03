#include <Replays/ReplayLoader.h>
#include <Replays/ReplaySaver.h>
#include <Replays/ReplayFrame.h>

USE_REPLAY_NS;

int main(){
	std::string INPUT_PATH = "replays"; //Can be replay or directory
	std::string OUTPUT_PATH = "3v3.json"; //Has to be json

	bool isDirectory = std::filesystem::is_directory(INPUT_PATH);

	if (not OUTPUT_PATH.ends_with(".json")) {
		VOID_ERR("OUTPUT_PATH must end with .json, current value: " << OUTPUT_PATH);
		exit(EXIT_FAILURE);
	}

	//Need that for interpolation
	RocketSim::Init("./collision_meshes");

	ReplaySaver saver = ReplaySaver();

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

	saver.SaveReplays(OUTPUT_PATH, replays);
}
