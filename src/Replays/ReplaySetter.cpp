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

	VOID_LOG("Loading replays...");
	ifs >> j;

	std::vector<Replay> replays = j.at("replays");

	VOID_LOG(replays.size() << " replay(s) loaded!");

	this->replays = replays;

	for (const Replay r: this->replays) {
		std::pair<int, int> pair = {};
		pair.first = r.metadata.nBlue;
		pair.second = r.metadata.nOrange;

		if (not this->sortedReplays.contains(pair)) {
			this->sortedReplays[pair] = { r };
		}
		else {
			this->sortedReplays[pair].push_back(r);
		}
	}

	VOID_LOG("Number of replays per mode: ");
	for (auto [key, val] : this->sortedReplays) {
		VOID_LOG(" - " << key.first << "v" << key.second << ": " << val.size());
	}
}

GameState ReplaySetter::ResetState(Arena* arena)
{

	std::pair<int, int> currentArenaConfig;

	int nBlue = 0, nOrange = 0;
	
	for (Car* c : arena->GetCars()) {
		c->team == Team::BLUE ? nBlue++ : nOrange++;
	}

	currentArenaConfig.first = nBlue;
	currentArenaConfig.second = nOrange;

	if (not this->sortedReplays.contains(currentArenaConfig)) return GameState(arena);

	int chosenReplayInd = RocketSim::Math::RandInt(0, this->sortedReplays[currentArenaConfig].size());
	Replay chosenReplay = this->sortedReplays[currentArenaConfig][chosenReplayInd];

	//TODO: An actually good way to pick the frame
	int chosenFrame = RocketSim::Math::RandInt(0, chosenReplay.metadata.numberOfPlayableFrames);
	
	GameState gs = chosenReplay.states[chosenFrame];

	VOID_LOG(gs.ball.pos.y);

	arena->ball->SetState(gs.ballState);
	auto cars = arena->GetCars();
	for (Car* c : cars) {
		for (const PlayerData p : gs.players) {
			if (p.carId == c->id) {
				c->SetState(p.carState);
				break;
			}
		}
	}
	
	return GameState(arena);
}