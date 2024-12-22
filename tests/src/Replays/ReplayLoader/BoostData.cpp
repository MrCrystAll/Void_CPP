#include <Replays/ReplayLoader/BoostData.h>

TEST_CASE("Boost data is correctly recalculated", VOID_REPLAYS_TAG) {
	ReplayLoader loader = ReplayLoader();

	Replay r = loader.LoadReplay("replays/tests/da07cdec-d058-48cd-add4-26dba8cebaba.replay", 150);

	RLGSC::GameState firstState = r.states[0];
	RLGSC::GameState boostState = r.states[13];
	RLGSC::GameState boostPickupState = r.states[12];

	int boosterId = r.metadata.GetPlayerWithId("17586584214334280299").match_id;

	SECTION("Boost timer & amount") {
		for (int i = 0; i < boostState.players.size(); i++) {

			if (boostState.players[i].carId == boosterId) {
				CHECK(boostState.players[i].carState.timeSpentBoosting == 14);
				CHECK_THAT(boostState.players[i].carState.boost, Catch::Matchers::WithinAbs(30.41, 0.1));
				CHECK_THAT(boostState.players[i].boostFraction, Catch::Matchers::WithinAbs(0.3041, 0.001));
			}
		}
	}

	SECTION("Boost pickups") {
		for (int i = 0; i < boostPickupState.players.size(); i++) {

			if (boostPickupState.players[i].carId == boosterId) {
				CHECK(boostPickupState.players[i].boostPickups == 1);
			}
		}
		
	}
}
