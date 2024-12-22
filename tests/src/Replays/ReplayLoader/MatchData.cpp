#include <Replays/ReplayLoader/MatchData.h>


TEST_CASE("Match data per player is being correctly assigned", VOID_REPLAYS_TAG) {
	ReplayLoader loader = ReplayLoader();

	Replay r = loader.LoadReplay("replays/tests/da07cdec-d058-48cd-add4-26dba8cebaba.replay", 150);

	RLGSC::GameState p1shotState = r.states[558];
	RLGSC::GameState p2shotState = r.states[755];

	RLGSC::GameState p1saveState = r.states[1197];
	RLGSC::GameState p2saveState = r.states[552];

	RLGSC::GameState p1goalState = r.states[4451];
	RLGSC::GameState p2goalState = r.states[2366];


	SECTION("Match shots") {
		CHECK(p1shotState.players[0].matchShots == 1);
		CHECK(p2shotState.players[1].matchShots == 1);
	}

	SECTION("Match saves") {
		CHECK(p1saveState.players[0].matchSaves == 1);
		CHECK(p2saveState.players[1].matchSaves == 1);
	}

	SECTION("Match goals") {
		CHECK(p1goalState.players[0].matchGoals == 1);
		CHECK(p2goalState.players[1].matchGoals == 1);
	}


	SECTION("Match assists") {
		Replay replay2s = loader.LoadReplay("replays/tests/ae6bcec7-d717-42af-904b-b77b32ac5c43.replay", 150);

		RLGSC::GameState p1AssistState = replay2s.states[1097];
		RLGSC::GameState p4AssistState = replay2s.states[3950];

		for (const RLGSC::PlayerData& p : p1AssistState.players) {
			if(p.carId == 1) CHECK(p.matchAssists == 1);
		}

		for (const RLGSC::PlayerData& p : p4AssistState.players) {
			if (p.carId == 6) CHECK(p.matchAssists == 1);
		}
	}
}
