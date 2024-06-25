#include "States/DoubleTap/DoubleTapState.h"

USE_DT_NS

static float random(float min, float max) {
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> distrib(min, max);
    return distrib(gen);
}

static Vec randomVec(Vec vec) {
    
    return Vec(random(0, vec.x), random(0, vec.y), random(0, vec.z));
}

static Angle randomAngle(const Angle vec) {
    const Angle angle = Angle(random(0, vec.yaw), random(0, vec.pitch), random(0, vec.roll));
    return angle;
}

RLGSC::GameState OverfittingDoubleTapSetter::ResetState(Arena* arena)
{
    BallState bs = {};
    bs.pos = { 600, 2000, 1300 };
    bs.vel = { -100, 2000, 600 };

    arena->ball->SetState(bs);

    for (Car* c : arena->GetCars()) {
        CarState cs = {};

        if (c->team == Team::BLUE) {
            cs.pos = { 500, 1500, 1500 };
            cs.vel = { 0, 1500, 500 };
            cs.rotMat = Angle(M_PI_2).ToRotMat();
            cs.hasFlipped = true;
            cs.hasJumped = true;
            cs.boost = 100;
        }
        else {
            cs.pos = { 0, RLGSC::CommonValues::BACK_WALL_Y, 17 };
        }

        c->SetState(cs);
    }

    return RLGSC::GameState(arena);
}

RLGSC::GameState DoubleTapState::ResetState(Arena* arena)
{
    Team chosenTeam = Team::BLUE;

    if (config.bothSides) {
        chosenTeam = random(0, 1) < 0.5 ? Team::BLUE : Team::ORANGE;
    }

    float yDir = 0;

    //Setting directions
    if (chosenTeam == Team::BLUE) {
        yDir = 1;
    }
    else {
        yDir = -1;
    }

    Vec ballInitialPos = Vec(0, 1000 * yDir, 1300);
    Vec agentInitialPos = Vec(0, 600 * yDir, 1100);

    Vec ballInitialVel = Vec(0, 2500 * yDir, 500);
    Vec agentInitialVel = Vec(0, 2000 * yDir, 900);
    Vec agentInitialAngVel = Vec(0, 0, 0);

    Angle agentInitialOrientation = Angle(M_PI_2);


    ballInitialPos += randomVec(config.ballVariance.posVariance);
    ballInitialPos.x -= config.ballVariance.posVariance.x / 2.0f;

    ballInitialVel += randomVec(config.ballVariance.velVariance);
    ballInitialVel.x -= config.ballVariance.velVariance.x / 2.0f;

    agentInitialPos += randomVec(config.carVariance.posVariance);
    agentInitialPos.x -= config.carVariance.posVariance.x / 2.0f;

    agentInitialVel += randomVec(config.carVariance.velVariance);
    agentInitialVel.x -= config.carVariance.velVariance.x / 2.0f;

    agentInitialAngVel += randomVec(config.carVariance.angVelVariance);
    ballInitialVel.x -= config.carVariance.angVelVariance.x / 2.0f;
    ballInitialVel.y -= config.carVariance.angVelVariance.y / 2.0f;
    ballInitialVel.z -= config.carVariance.angVelVariance.z / 2.0f;


    agentInitialOrientation = agentInitialOrientation + randomAngle(config.carVariance.orientVariance);

    BallState bs = {};
    bs.pos = ballInitialPos;
    bs.vel = ballInitialVel;

    arena->ball->SetState(bs);

    for (Car* c : arena->GetCars()) {
        CarState cs = {};

        if (chosenTeam == c->team) {
            cs.pos = agentInitialPos;
            cs.vel = agentInitialVel;
            cs.rotMat = agentInitialOrientation.ToRotMat();
            cs.angVel = agentInitialAngVel;
            cs.hasFlipped = true;
            cs.hasJumped = true;
        }
        else {
            cs.pos = Vec(0, RLGSC::CommonValues::BACK_WALL_Y * yDir, 17);
        }

        cs.boost = 100;

        c->SetState(cs);
    }

    return RLGSC::GameState(arena);
}
