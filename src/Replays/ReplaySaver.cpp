#include <Replays/ReplaySaver.h>

USE_REPLAY_NS;

json ReplaySaver::SerializeReplay(Replay replay)
{
	return replay;
}

void ReplaySaver::SaveReplay(std::string path, Replay replay)
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

void ReplaySaver::SaveReplays(std::string path, std::vector<Replay> replays)
{
	if (std::filesystem::exists(path)) {
		REPLAY_SAVER_LOG("A file already exists at " << path << ". Overwriting.");
	}

	REPLAY_SAVER_LOG("Saving " << replays.size() << " replay(s)...");

	json jReplays = replays;
	std::ofstream ofs = std::ofstream(path, std::ofstream::trunc);
	ofs << jReplays;
	ofs.close();

	REPLAY_SAVER_LOG(replays.size() << " replay(s) successfully saved at " << path);
}
