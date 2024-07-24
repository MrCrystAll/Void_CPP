#pragma once

#include <Replays/ReplayFrame.h>
#include <Replays/ReplayUtils.h>

START_REPLAY_NS

#define REPLAY_SAVER_LOG(content) VOID_LOG("[REPLAY SAVER] " << content)
#define REPLAY_SAVER_WARN(content) VOID_WARN("[REPLAY SAVER] " << content)
#define REPLAY_SAVER_ERR(content) VOID_ERR("[REPLAY SAVER] " << content)

class ReplaySaver {
public:

	json SerializeReplay(Replay replay);

	void SaveReplay(std::string path, Replay replay);
	void SaveReplays(std::string path, std::vector<Replay> replays);
};

END_REPLAY_NS