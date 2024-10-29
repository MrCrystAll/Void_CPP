#include <Replays/ReplayLoader/PhysicsState.h>
#include <RLGymSim_CPP/Utils/BasicTypes/Quat.h>


ReplayLoader loader = ReplayLoader();

TEST_CASE("Physical state is correctly converted", VOID_REPLAYS_TAG) {
	Replay r = loader.LoadReplay("replays/tests/da07cdec-d058-48cd-add4-26dba8cebaba.replay", 150);

	RLGSC::GameState firstState = r.states[0];
	RLGSC::GameState angVelState = r.states[13];

	//Positions
	CHECK_THAT(firstState.players[0].phys.pos.x, Catch::Matchers::WithinAbs(2048.0, 0.5));
	CHECK_THAT(firstState.players[0].phys.pos.y, Catch::Matchers::WithinAbs(-2560.0, 0.5));
	CHECK_THAT(firstState.players[0].phys.pos.z, Catch::Matchers::WithinAbs(RLConst::CAR_SPAWN_REST_Z, 0.1));

	CHECK_THAT(firstState.players[1].phys.pos.x, Catch::Matchers::WithinAbs(-2048.0, 0.5));
	CHECK_THAT(firstState.players[1].phys.pos.y, Catch::Matchers::WithinAbs(2560.0, 0.5));
	CHECK_THAT(firstState.players[1].phys.pos.z, Catch::Matchers::WithinAbs(RLConst::CAR_SPAWN_REST_Z, 0.1));

	//Velocities
	CHECK_THAT(firstState.players[0].phys.vel.x, Catch::Matchers::WithinAbs(9.42, 0.5));
	CHECK_THAT(firstState.players[0].phys.vel.y, Catch::Matchers::WithinAbs(-9.42, 0.5));
	CHECK_THAT(firstState.players[0].phys.vel.z, Catch::Matchers::WithinAbs(0.27, 0.5));

	CHECK_THAT(firstState.players[1].phys.vel.x, Catch::Matchers::WithinAbs(15.27, 0.5));
	CHECK_THAT(firstState.players[1].phys.vel.y, Catch::Matchers::WithinAbs(-15.27, 0.5));
	CHECK_THAT(firstState.players[1].phys.vel.z, Catch::Matchers::WithinAbs(0.19, 0.5));

	//Ang vels
	CHECK_THAT(angVelState.players[0].phys.angVel.x, Catch::Matchers::WithinAbs(0.1565, 0.2));
	CHECK_THAT(angVelState.players[0].phys.angVel.y, Catch::Matchers::WithinAbs(0.1317, 0.2));
	CHECK_THAT(angVelState.players[0].phys.angVel.z, Catch::Matchers::WithinAbs(-0.8199, 0.2));

	CHECK_THAT(angVelState.players[1].phys.angVel.x, Catch::Matchers::WithinAbs(-0.0034, 0.2));
	CHECK_THAT(angVelState.players[1].phys.angVel.y, Catch::Matchers::WithinAbs(-0.0031, 0.2));
	CHECK_THAT(angVelState.players[1].phys.angVel.z, Catch::Matchers::WithinAbs(0.6861, 0.2));

	//Rotations
	RotMat rotMat = angVelState.players[0].phys.rotMat;

	float q[4];

	q[0] = 0.4114;
	q[1] = -0.003;
	q[2] = 0.0012;
	q[3] = 0.9114;

	RLGSC::Quat stateRot = RLGSC::Quat::FromRotMat(rotMat);
	CHECK_THAT(stateRot.w, Catch::Matchers::WithinAbs(q[0], 0.01));
	CHECK_THAT(stateRot.x, Catch::Matchers::WithinAbs(q[1], 0.01));
	CHECK_THAT(stateRot.y, Catch::Matchers::WithinAbs(q[2], 0.01));
	CHECK_THAT(stateRot.z, Catch::Matchers::WithinAbs(q[3], 0.01));

	rotMat = angVelState.players[1].phys.rotMat;

	q[0] = 0.9258;
	q[1] = 0.0018;
	q[2] = 0.0043;
	q[3] = -0.3777;

	stateRot = RLGSC::Quat::FromRotMat(rotMat);
	CHECK_THAT(stateRot.w, Catch::Matchers::WithinAbs(q[0], 0.01));
	CHECK_THAT(stateRot.x, Catch::Matchers::WithinAbs(q[1], 0.01));
	CHECK_THAT(stateRot.y, Catch::Matchers::WithinAbs(q[2], 0.01));
	CHECK_THAT(stateRot.z, Catch::Matchers::WithinAbs(q[3], 0.01));

}
