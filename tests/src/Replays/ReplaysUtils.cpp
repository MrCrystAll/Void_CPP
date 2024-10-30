#include <Replays/ReplaysUtils.h>
#include <Replays/RCF/RCFUtils.h>
#include <catch2/catch_all.hpp>

RLGSC::GameState StateGenerator::CreateGroundState(int nBluePlayers, int nOrangePlayers)
{
    Arena* arena = StateGenerator::CreateArena(nBluePlayers, nOrangePlayers);
    for (Car* c : arena->GetCars()) {
        CarState cs = {};
        cs.pos = { -1700, -2000, RLConst::CAR_SPAWN_REST_Z };
        cs.isOnGround = true;

        c->SetState(cs);
        break;
    }

    return RLGSC::GameState(arena);
}

RLGSC::GameState StateGenerator::CreateAirState(int nBluePlayers, int nOrangePlayers)
{
    Arena* arena = StateGenerator::CreateArena(nBluePlayers, nOrangePlayers);
    for (Car* c : arena->GetCars()) {
        CarState cs = {};
        cs.pos = { -1700, -2000, 650 };
        cs.isOnGround = false;

        c->SetState(cs);
        break;
    }

    return RLGSC::GameState(arena);
}

RLGSC::GameState StateGenerator::CreateOnWallState(int nBluePlayers, int nOrangePlayers)
{
    Arena* arena = StateGenerator::CreateArena(nBluePlayers, nOrangePlayers);
    for (Car* c : arena->GetCars()) {
        CarState cs = {};
        cs.pos = { - RLGSC::CommonValues::SIDE_WALL_X, 2000, 650 };
        Angle angle = Angle(0, 0, -M_PI_2);

        cs.rotMat = angle.ToRotMat();
        cs.isOnGround = true;

        c->SetState(cs);
        break;
    }

    return RLGSC::GameState(arena);
}

RLGSC::GameState StateGenerator::CreateOnCeilingState(int nBluePlayers, int nOrangePlayers)
{
    Arena* arena = StateGenerator::CreateArena(nBluePlayers, nOrangePlayers);
    for (Car* c : arena->GetCars()) {
        CarState cs = {};
        cs.pos = { -1700, 2000, RLGSC::CommonValues::CEILING_Z };
        Angle angle = Angle(0, 0, M_PI);

        cs.rotMat = angle.ToRotMat();
        cs.isOnGround = true;

        c->SetState(cs);
        break;
    }

    return RLGSC::GameState(arena);
}

RLGSC::GameState StateGenerator::CreateOnCornerState(int nBluePlayers, int nOrangePlayers)
{
    Arena* arena = StateGenerator::CreateArena(nBluePlayers, nOrangePlayers);
    for (Car* c : arena->GetCars()) {
        CarState cs = {};
        cs.pos = { 3600, -4464, 650 };
        Angle angle = Angle(M_PI_4, 0, -M_PI_2);

        cs.rotMat = angle.ToRotMat();
        cs.isOnGround = true;

        c->SetState(cs);
        break;
    }

    return RLGSC::GameState(arena);
}

RocketSim::Arena* StateGenerator::CreateArena(int nBluePlayers, int nOrangePlayers)
{
    RocketSim::Init("./collision_meshes");
    Arena* arena = RocketSim::Arena::Create(GameMode::SOCCAR);

    for (int i = 0; i < nBluePlayers; i++) {
        arena->AddCar(Team::BLUE);
    }
    for (int i = 0; i < nOrangePlayers; i++) {
        arena->AddCar(Team::ORANGE);
    }

    return arena;
}

TEST_CASE("States are correctly created", VOID_REPLAYS_TAG) {

    SECTION("Ground state") {
        RLGSC::GameState groundState = StateGenerator::CreateGroundState(1, 1);

        CHECK(groundState.players[0].carState.isOnGround);
        CHECK_THAT(groundState.players[0].phys.pos.z, Catch::Matchers::WithinAbs(RLConst::CAR_SPAWN_REST_Z, 0.5));
    }
    
    SECTION("Air state") {
        RLGSC::GameState airState = StateGenerator::CreateAirState(1, 1);

        CHECK(!airState.players[0].carState.isOnGround);
        CHECK(airState.players[0].phys.pos.z > 100);
    }

    SECTION("Wall state") {
        RLGSC::GameState wallState = StateGenerator::CreateOnWallState(1, 1);

        CHECK(wallState.players[0].carState.isOnGround);
        CHECK(Void::Replays::RCF::IsOnWall(wallState.players[0]));
    }

    SECTION("Ceiling state") {
        RLGSC::GameState ceilingState = StateGenerator::CreateOnCeilingState(1, 1);

        CHECK(ceilingState.players[0].carState.isOnGround);
        CHECK(Void::Replays::RCF::IsOnCeiling(ceilingState.players[0]));
    }

    SECTION("Corner state") {
        RLGSC::GameState cornerState = StateGenerator::CreateOnCornerState(1, 1);
        CHECK(cornerState.players[0].carState.isOnGround);
        CHECK(Void::Replays::RCF::isOnCorner(cornerState.players[0]));
    }
}
