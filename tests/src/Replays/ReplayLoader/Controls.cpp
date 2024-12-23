#include <Replays/ReplayLoader/Controls.h>

TEST_CASE("Controls are correctly reproduced", VOID_REPLAYS_TAG) {
	ReplayLoader loader = ReplayLoader();

	Replay r = loader.LoadReplay("replays/tests/da07cdec-d058-48cd-add4-26dba8cebaba.replay", 150);

	int p1Id = r.metadata.GetPlayerWithId("11024088351317006949").match_id;
	RLGSC::GameState beforeFlipState = r.states[13];
	RLGSC::GameState afterFlipState = r.states[15];


	SECTION("Known inputs") {
		for (const RLGSC::PlayerData& pData : beforeFlipState.players) {
			if (pData.carId == p1Id) {
				CHECK_THAT(pData.carState.lastControls.throttle, Catch::Matchers::WithinAbs(0.0, 0.01));
				CHECK_THAT(pData.carState.lastControls.steer, Catch::Matchers::WithinAbs(0.58, 0.01));
				CHECK_THAT(pData.carState.lastControls.pitch, Catch::Matchers::WithinAbs(0.0, 0.01));
				CHECK_THAT(pData.carState.lastControls.yaw, Catch::Matchers::WithinAbs(-0.17, 0.01));
				CHECK_THAT(pData.carState.lastControls.roll, Catch::Matchers::WithinAbs(0.02, 0.01));
				CHECK_THAT(pData.carState.lastControls.jump, Catch::Matchers::WithinAbs(0.0, 0.01));
				CHECK_THAT(pData.carState.lastControls.boost, Catch::Matchers::WithinAbs(0.0, 0.01));
				CHECK_THAT(pData.carState.lastControls.handbrake, Catch::Matchers::WithinAbs(0.0, 0.01));
			}
		}
	}
}
