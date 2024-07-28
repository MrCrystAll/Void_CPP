#include <Replays/CarballHandling.h>

START_REPLAY_NS

int CallCarball(std::string replayPath, std::string output_path) {
	std::string carballCommand = "carball.exe csv -i ";
	int retCode = system((carballCommand + replayPath + " -o " + output_path + " > nul 2> nul").c_str());
	if (retCode != 0) {
		VOID_ERR("carball couldn't convert replay and exited with code " << retCode);
	}

	return retCode;
}

END_REPLAY_NS