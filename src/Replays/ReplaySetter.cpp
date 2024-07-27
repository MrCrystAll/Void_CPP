#include <Replays/ReplaySetter.h>
#include <Replays/ReplayLoader.h>
#include <RLGymSim_CPP/Math.h>

USE_REPLAY_NS;

ReplaySetter::ReplaySetter(ReplaySetterArgs args)
{
	if (args.loadExistingReplays.toggle) {
		for (std::string path : args.loadExistingReplays.paths) {
			if (not std::filesystem::exists(path)) {
				REPLAY_LOADER_WARN("The path " << path << " doesn't exist, no replays loaded");
			}
			else {
				std::ifstream ifs = std::ifstream(path);
				json j;

				REPLAY_LOADER_LOG("Loading replays for path " << path << "...");
				ifs >> j;

				ifs.close();

				std::vector<Replay> replays = j;

				REPLAY_LOADER_LOG(replays.size() << " replay(s) loaded!");

				this->replays.insert(this->replays.end(), replays.begin(), replays.end());
			}
		}
	}

	if (args.loadNewReplays.toggle) {
		// Initialize RocketSim with collision meshes
		RocketSim::Init("./collision_meshes");

		ReplayLoader loader = ReplayLoader();

		for (std::string path : args.loadNewReplays.paths) {
			if (path.ends_with(".replay")) {
				Replay r = loader.LoadReplay(path, args.loadNewReplays.endDelay, not args.saveReplays);
				this->replays.push_back(r);
			}
			else {
				std::vector<Replay> replays = loader.LoadReplays(path, args.loadNewReplays.endDelay, not args.saveReplays);

				//Concat current replays and loaded replays
				this->replays.insert(this->replays.end(), replays.begin(), replays.end());
			}
		}
	}

	if (args.saveReplays) {
		ReplaySaver().SaveReplays(args.outputPath, this->replays);
	}

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

	if (not this->replays.empty()) {
		REPLAY_LOADER_LOG("Number of replays per mode: ");
		for (auto [key, val] : this->sortedReplays) {
			REPLAY_LOADER_LOG(" - " << key.first << "v" << key.second << ": " << val.size());
		}
	}
	else {
		REPLAY_LOADER_LOG("No replays were loaded :(");
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

	if (not this->sortedReplays.contains(currentArenaConfig)) {
		REPLAY_LOADER_WARN("No states for gamemode " << currentArenaConfig.first << "v" << currentArenaConfig.second << ". Reset is ignored.");
		return GameState(arena);
	}
		

	int chosenReplayInd = RocketSim::Math::RandInt(0, this->sortedReplays[currentArenaConfig].size());
	Replay chosenReplay = this->sortedReplays[currentArenaConfig][chosenReplayInd];

	int chosenFrame = RocketSim::Math::RandInt(0, chosenReplay.metadata.numberOfPlayableFrames);
	
	GameState gs = chosenReplay.states[chosenFrame];

	arena->ball->SetState(gs.ballState);
	auto cars = arena->GetCars();
	int i = 0;
	for (Car* c : cars) {
		c->SetState(gs.players[i].carState);
		i++;
	}
	
	return GameState(arena);
}