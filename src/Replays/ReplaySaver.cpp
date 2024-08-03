#include <Replays/ReplaySaver.h>

USE_REPLAY_NS;

json ReplaySaver::SerializeReplay(Replay replay)
{
	return replay;
}

void ReplaySaver::SaveReplay(std::string path, Replay replay, bool overwrite)
{
	if (std::filesystem::exists(path)) {
		REPLAY_SAVER_LOG("A file already exists at " << path << ". Overwriting.");
	}
	std::ofstream ofs = std::ofstream(path, std::ofstream::trunc);
	std::vector<json> vectoredReplay = { SerializeReplay(replay)};
	ofs << vectoredReplay;
	ofs.close();

	REPLAY_SAVER_LOG("Replay successfully saved at " << path);
}

void ReplaySaver::SaveReplays(std::string path, std::vector<Replay> replays, bool overwrite)
{
	std::vector<Replay> additionalReplays = {};
	if (std::filesystem::exists(path)) {
		if(overwrite) REPLAY_SAVER_LOG("A file already exists at " << path << ". Overwriting.");
		else {
			std::ifstream ifs = std::ifstream(path);
			if(!std::filesystem::is_empty(path)){
				json j;
				REPLAY_SAVER_LOG("Getting existing data from \"" << path << "\"...");
				ifs >> j;
				additionalReplays = j;
				REPLAY_SAVER_LOG("Got " << additionalReplays.size() << " replay(s) from \"" << path << "\"");
			}
			else {
				REPLAY_SAVER_WARN(path << " is empty. Ignoring.");
			}
			
		}
	}

	additionalReplays.insert(additionalReplays.end(), replays.begin(), replays.end());

	REPLAY_SAVER_LOG("Saving " << additionalReplays.size() << " replay(s)...");

	json jReplays = additionalReplays;
	std::ofstream ofs = std::ofstream(path, std::ofstream::trunc);
	ofs << jReplays;
	ofs.close();

	REPLAY_SAVER_LOG(additionalReplays.size() << " replay(s) successfully saved at " << path);
}
