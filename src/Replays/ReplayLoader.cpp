#include <Replays/ReplayLoader.h>
#include <RLGymSim_CPP/Gym.h>

#include <ctime>

#include <RLGymSim_CPP/Utils/RewardFunctions/CommonRewards.h>
#include <RLGymSim_CPP/Utils/OBSBuilders/DefaultOBS.h>
#include <RLGymSim_CPP/Utils/ActionParsers/DiscreteAction.h>

USE_REPLAY_NS;

ReplayLoader::ReplayLoader()
{
	RocketSim::Init("./collision_meshes");
}

Replay ReplayLoader::LoadReplay(std::string path, int endDelay, bool saveReplay)
{
	int retCode = CallCarball(path);

	ConvertedReplay replay = ConvertedReplay();

	if (retCode != 0) return Replay();

	
	ReplayAnalysis analysis = this->LoadAnalysis(DEFAULT_CARBALL_RESULT_PATH, endDelay);
	ReplayMetadata metadata = this->LoadMetadata(DEFAULT_CARBALL_RESULT_PATH, analysis);

	REPLAY_LOADER_LOG("Loaded metadata and analysis");

	std::vector<GameFrame> gameFrames = this->LoadGameFrames(DEFAULT_CARBALL_RESULT_PATH, analysis);
	REPLAY_LOADER_LOG("Found " << gameFrames.size() << " game frames");
	std::vector<BallFrame> ballFrames = this->LoadBallFrames(DEFAULT_CARBALL_RESULT_PATH, analysis);
	REPLAY_LOADER_LOG("Found " << ballFrames.size() << " ball frames");
	std::vector<std::vector<PlayerFrame>> playersFrames = this->LoadPlayersFrames(DEFAULT_CARBALL_RESULT_PATH, analysis);
	REPLAY_LOADER_LOG("Found " << playersFrames.size() << " group of players frames");

	for (int i = 0; i < gameFrames.size(); i++) {
		ReplayFrame rf = {};

		rf.ball = ballFrames[i];
		rf.game = gameFrames[i];
		rf.players = playersFrames[i];

		replay.AddFrame(rf);
	}

	replay.metadata = metadata;
	replay.analysis = analysis;

	Replay finalReplay = {};
	finalReplay.analysis = replay.analysis;
	finalReplay.metadata = replay.metadata;

	finalReplay.states = this->InterpolateReplays(replay);

	if(saveReplay) replaySaver.SaveReplay(GetTimePath() + ".json", finalReplay);

	return finalReplay;
}

std::vector<Replay> ReplayLoader::LoadReplays(std::string path, int delay, bool saveReplay)
{
	std::vector<Replay> replays = {};
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		const auto filenameStr = entry.path().filename().string();

		if (filenameStr.ends_with(".replay")) {
			Replay r = LoadReplay(path + "/" + filenameStr, delay);
			if (r.states.size() == 0) continue;
			replays.push_back(r);
		}
	}

	if(saveReplay and not replays.empty()) replaySaver.SaveReplays(GetTimePath() + ".json", replays);

	return replays;
}

std::vector<GameFrame> ReplayLoader::LoadGameFrames(std::string path, ReplayAnalysis analysis)
{
	std::string gameFramesFile = path + "/__game.csv";
	//At this moment, the new replay is stored
	io::CSVReader<6> gameReader = io::CSVReader<6>(gameFramesFile);

	float time, delta;
	int secondsRemaining, replicatedGameStateTimeRemaining;
	int isOvertime, ballHasBeenHit;

	gameReader.read_header(io::ignore_missing_column, "time", "delta", "seconds_remaining", "replicated_game_state_time_remaining", "is_overtime", "ball_has_been_hit");

	std::vector<GameFrame> gameFrames = {};

	int i = 0;

	//Game parsing
	while (gameReader.read_row(time, delta, secondsRemaining, replicatedGameStateTimeRemaining, isOvertime, ballHasBeenHit)) {
		GameFrame gf = {};

		if (not analysis.IsFrameInAnyPeriod(i)) {
			i++;
			continue;
		}

		gf.time = time;
		gf.delta = delta;
		gf.secondsRemaining = secondsRemaining;
		gf.replicatedGameStateTimeRemaining = replicatedGameStateTimeRemaining;
		gf.isOvertime = isOvertime;
		gf.ballHasBeenHit = ballHasBeenHit;

		gameFrames.push_back(gf);
		i++;
	}
	return gameFrames;
}

std::vector<BallFrame> ReplayLoader::LoadBallFrames(std::string path, ReplayAnalysis analysis)
{
	std::string ballFramesFile = path + "/__ball.csv";

	//At this moment, the new replay is stored
	io::CSVReader<15> gameReader = io::CSVReader<15>(ballFramesFile);

	//Got to keep this one to convert it to bool
	int isSleeping;

	gameReader.read_header(io::ignore_missing_column, "is_sleeping", "pos_x", "pos_y", "pos_z", "vel_x", "vel_y", "vel_z", "quat_w", "quat_x", "quat_y", "quat_z", "ang_vel_x", "ang_vel_y", "ang_vel_z", "hit_team_num");

	std::vector<BallFrame> ballFrames = {};
	BallFrame bf = {};

	int i = 0;

	//Game parsing
	while (gameReader.read_row(isSleeping, bf.pos.x, bf.pos.y, bf.pos.z, bf.vel.x, bf.vel.y, bf.vel.z, bf.rot._w, bf.rot.x, bf.rot.y, bf.rot.z, bf.rot.x, bf.rot.y, bf.rot.z, bf.hitTeamNum)) {
		if (not analysis.IsFrameInAnyPeriod(i)) {
			i++;
			continue;
		}

		bf.isSleeping = isSleeping;

		ballFrames.push_back(bf);
		bf = {};
		i++;
	}

	return ballFrames;
}

std::vector<std::vector<PlayerFrame>> ReplayLoader::LoadPlayersFrames(std::string path, ReplayAnalysis analysis)
{
	std::vector<std::vector<PlayerFrame>> playerFrames = {};

	//First, find all players
	std::ifstream ifs(path + "/metadata.json");
	json jf = json::parse(ifs);

	auto players = jf["players"];
	std::vector<std::string> playerFiles = {};

	int nbPlayers = players.size();

	std::vector<std::string> bluePlayers = {}, orangePlayers = {};

	//Get their file
	for (auto& p : players) {
		std::string playerUniqueId = p.at("unique_id");
		if (p.at("is_orange")) orangePlayers.push_back(playerUniqueId);
		else bluePlayers.push_back(playerUniqueId);
	}

	for (std::string pUID : bluePlayers) {
		playerFiles.push_back("player_" + pUID + ".csv");
	}

	for (std::string pUID : orangePlayers) {
		playerFiles.push_back("player_" + pUID + ".csv");
	}

	//Profit
	int i = 0;


	for (std::string playerPath : playerFiles) {
		int playersFrameIn = 0;
		int totalPlayersFrameIn = 0;
		std::string playerFilePath = path + "/" + playerPath;

		io::CSVReader<37> reader = io::CSVReader<37>(playerFilePath);

		reader.read_header(io::ignore_missing_column, "is_sleeping", "pos_x", "pos_y", "pos_z", "vel_x", "vel_y", "vel_z", "quat_w", "quat_x", "quat_y", "quat_z", "ang_vel_x", "ang_vel_y", "ang_vel_z", "throttle", "steer", "handbrake", "match_score", "match_goals", "match_assists", "match_saves", "match_shots", "team", "ping", "boost_is_active", "boost_amount", "boost_pickup", "jump_is_active", "double_jump_is_active", "flip_car_is_active", "dodge_is_active", "double_jump_torque_x", "double_jump_torque_y", "double_jump_torque_z", "dodge_torque_x", "dodge_torque_y", "dodge_torque_z");

		//Forgive me for that
		float pos_x, pos_y, pos_z, vel_x, vel_y, vel_z, rot_x, rot_y, rot_z, rot_w, angVel_x, angVel_y, angVel_z, boost_amount, double_jump_torque_x, double_jump_torque_y, double_jump_torque_z, dodge_torque_x, dodge_torque_y, dodge_torque_z;
		int isSleeping, throttle, steer, match_score, match_goals, match_assists, match_saves, match_shots, team, ping, boost_is_active, boost_pickup, jump_is_active, double_jump_is_active, flip_car_is_active, dodge_is_active;
		std::string handbrake;

		while (reader.read_row(isSleeping, pos_x, pos_y, pos_z, vel_x, vel_y, vel_z, rot_w, rot_x, rot_y, rot_z, angVel_x, angVel_y, angVel_z, throttle, steer, handbrake, match_score, match_goals, match_assists, match_saves, match_shots, team, ping, boost_is_active, boost_amount, boost_pickup, jump_is_active, double_jump_is_active, flip_car_is_active, dodge_is_active, double_jump_torque_x, double_jump_torque_y, double_jump_torque_z, dodge_torque_x, dodge_torque_y, dodge_torque_z)) {
			PlayerFrame pf = {};

			if (not analysis.IsFrameInAnyPeriod(totalPlayersFrameIn)) {
				totalPlayersFrameIn++;
				continue;
			}

			pf.pos = Vec(pos_x, pos_y, pos_z);
			pf.vel = Vec(vel_x, vel_y, vel_z);
			pf.angVel = Vec(angVel_x, angVel_y, angVel_z);
			pf.rot = Vec(rot_x, rot_y, rot_z, rot_w);

			pf.isSleeping = isSleeping;
			pf.throttle = throttle;
			pf.steer = steer;
			std::istringstream(handbrake) >> std::boolalpha >> pf.handbrake;

			pf.matchScore = match_score;
			pf.matchAssists = match_assists;
			pf.matchGoals = match_goals;
			pf.matchSaves = match_saves;
			pf.matchShots = match_shots;

			pf.dodgeJumpTorque = Vec(double_jump_torque_x, double_jump_torque_y, double_jump_torque_z);
			pf.dodgeTorque = Vec(dodge_torque_x, dodge_torque_y, dodge_torque_z);

			pf.isBoostActive = boost_is_active;
			pf.isDodgeActive = dodge_is_active;
			pf.isDoubleJumpActive = double_jump_is_active;
			pf.isFlipCarActive = flip_car_is_active;
			pf.isJumpActive = jump_is_active;

			pf.team = team;

			pf.boostAmount = boost_amount;
			pf.boostPickup = boost_pickup;

			if (i == 0) {
				playerFrames.push_back({ pf });
			}
			else {
				playerFrames[playersFrameIn].push_back(pf);
			}

			playersFrameIn++;
			totalPlayersFrameIn++;

		}
		i++;
	}

	return playerFrames;
}

ReplayMetadata ReplayLoader::LoadMetadata(std::string path, ReplayAnalysis analysis)
{
	std::ifstream ifs(path + "/metadata.json");
	json metadata = json::parse(ifs);


	ReplayMetadata replayMetadata = {};
	replayMetadata.numberOfFrames = metadata.at("game").at("num_frames");
	replayMetadata.numberOfPlayableFrames = analysis.GetNumberOfPlayableFrames();

	auto goalFrames = metadata.at("game").at("goals");

	RLGSC::ScoreLine scoreLine = RLGSC::ScoreLine();

	auto jBlueScore = metadata.at("game").at("team_0_score");
	int blueScore = 0;

	auto jOrangeScore = metadata.at("game").at("team_1_score");
	int orangeScore = 0;

	if (!jBlueScore.is_null()) blueScore = jBlueScore;
	if (!jOrangeScore.is_null()) orangeScore = jOrangeScore;

	scoreLine[(int)Team::BLUE] = blueScore;
	scoreLine[(int)Team::ORANGE] = orangeScore;

	replayMetadata.scoreLine = scoreLine;

	std::vector<GoalFrame> metadataGoalFrames = {};

	for (size_t i = 0; i < goalFrames.size(); i++)
	{
		GoalFrame gf = {};
		gf.frame = goalFrames[i].at("frame");
		gf.team = goalFrames[i].at("is_orange") == "true" ? Team::BLUE : Team::ORANGE;

		metadataGoalFrames.push_back(gf);
	}

	for (auto const& player : metadata.at("players")) {
		bool isOrange = player.at("is_orange");
		isOrange ? replayMetadata.nOrange++ : replayMetadata.nBlue++;
	}

	replayMetadata.goalFrames = metadataGoalFrames;
	return replayMetadata;
}

ReplayAnalysis ReplayLoader::LoadAnalysis(std::string path, int endDelay)
{
	std::ifstream ifs(path + "/analyzer.json");
	json jsonAnalysis = json::parse(ifs);

	ReplayAnalysis analysis = {};

	
	for (auto const& gp : jsonAnalysis.at("gameplay_periods")) {
		GameplayPeriod gameplayPeriod = {};
		gameplayPeriod.startFrame = gp.at("start_frame");
		gameplayPeriod.endFrame = gp.at("end_frame");
		gameplayPeriod.firstHitFrame = gp.at("first_hit_frame");
		if (gp.at("goal_frame").is_null()) {
			gameplayPeriod.goalFrame = -1;
		}
		else {
			gameplayPeriod.goalFrame = gp.at("goal_frame");
		}

		analysis.gameplayPeriods.push_back(gameplayPeriod);
	}

	return analysis;
}

void ReplayFrameToState(ReplayFrame frame, Arena* arena)
{
	BallState bs = BallFrame::ToBallState(frame.ball);
	arena->ball->SetState(bs);

	int i = 0;
	for (Car* c : arena->GetCars()) {
		CarState cs = PlayerFrame::ToCarState(frame.players[i]);
		c->SetState(cs);
		i++;
	}
}

std::vector<RLGSC::GameState> ReplayLoader::InterpolateReplays(ConvertedReplay replay)
{
	//Fuck that already
	int nbBlue, nbOrange;

	nbBlue = replay.metadata.nBlue;
	nbOrange = replay.metadata.nOrange;

	RocketSim::Arena* arena = RocketSim::Arena::Create(GameMode::SOCCAR);
	for (int i = 0; i < nbBlue; i++) {
		arena->AddCar(Team::BLUE);
	}
	for (int i = 0; i < nbOrange; i++) {
		arena->AddCar(Team::ORANGE);
	}
	
	std::vector<RLGSC::GameState> states = std::vector<RLGSC::GameState>(replay.metadata.numberOfPlayableFrames);
	int i = 0;

	REPLAY_LOADER_LOG("Interpolating...");
	for (const ReplayFrame rf : replay.frames) {
		ReplayFrameToState(rf, arena);
		arena->Step();
		states[i] = RLGSC::GameState(arena);
		i++;
	}
	REPLAY_LOADER_LOG("Interpolation complete");

	return states;
}
