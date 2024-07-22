#include <Replays/ReplaySetter.h>
#include <Replays/ReplayLoader.h>
#include <RLGymSim_CPP/Math.h>

USE_REPLAY_NS;

ReplaySetter::ReplaySetter(std::string replaysToLoad)
{
	if (not std::filesystem::exists(replaysToLoad)) {
		VOID_WARN("The path " << replaysToLoad << " doesn't exist, no replays loaded");
		return;
	}
	std::ifstream ifs = std::ifstream(replaysToLoad);
	json j;
	ifs >> j;

	Replay replay;
	replay.from_json(j, replay);

	this->replays.push_back(replay);
}

GameState ReplaySetter::ResetState(Arena* arena)
{
	if (this->replays.size() == 0) return GameState(arena);
	//TODO: An actually good way to pick a replay
	int chosenReplayInd = RocketSim::Math::RandInt(0, this->replays.size());
	Replay chosenReplay = this->replays[chosenReplayInd];

	//TODO: An actually good way to pick the frame
	int chosenFrame = RocketSim::Math::RandInt(0, chosenReplay.metadata.numberOfPlayableFrames);
	
	this->ReplayFrameToState(chosenReplay.frames[chosenFrame], arena);
	return GameState(arena);
}

void ReplaySetter::ReplayFrameToState(ReplayFrame frame, Arena* arena)
{
	BallState bs = BallFrame::ToBallState(frame.ball);
	arena->ball->SetState(bs);

	int i = 0;
	for (Car* c: arena->GetCars()) {
		PlayerData pd = PlayerFrame::ToPlayerData(frame.players[i]);
		c->SetState(pd.carState);
		i++;
	}
}
