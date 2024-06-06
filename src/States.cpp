#include "States.h"

WallPinchSetter::WallPinchSetter(float posVariance, float velVariance, float orientVariance): posVariance(posVariance), velVariance(velVariance), orientVariance(orientVariance) {

}

RLGSC::GameState WallPinchSetter::ResetState(Arena* arena)
{
    BallState bs = {};
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(0, 1);

    short dir = distrib(gen) == 0 ? -1 : 1;

    std::uniform_int_distribution<> distribPos(0, posVariance);
    std::uniform_int_distribution<> distribVel(0, velVariance);
    std::uniform_int_distribution<> distribOrient(-orientVariance, orientVariance);
    int ballPosX = (distribPos(gen) + 2000) * dir;
    int ballVelX = (distribVel(gen) + 1400) * dir;

    int carPosX = (distribPos(gen) + 300) * dir * -1 + ballPosX;
    int carVelX = (distribVel(gen) + 300) * dir;

    float carYaw = (0.9 + distribOrient(gen)) * M_PI;
    carYaw = dir == -1 ? carYaw : M_PI - carYaw;
    
    bs.pos = Vec(ballPosX, -3000, RLGSC::CommonValues::BALL_RADIUS);
    bs.vel = Vec(ballVelX, 400, 10);
    bs.angVel = Vec(0, 0, 0);

    arena->ball->SetState(bs);

    for (Car* car : arena->_cars) {
        CarState cs = {};
        cs.pos = Vec(carPosX, -3200, 20);
        cs.vel = Vec(carVelX, 400, 10);
        cs.angVel = Vec(0, 0, 0);
        Angle angle = Angle(carYaw, 0, 0);
        cs.rotMat = angle.ToRotMat();
        cs.boost = 100;
        car->SetState(cs);
    }


    return RLGSC::GameState(arena);
}

CeilingPinchSetter::CeilingPinchSetter(float posVariance, float velVariance, float orientVariance) : posVariance(posVariance), velVariance(velVariance), orientVariance(orientVariance)
{
}

RLGSC::GameState CeilingPinchSetter::ResetState(Arena* arena)
{
    BallState bs = {};
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(0, 1);

    short dir = distrib(gen) == 0 ? -1 : 1;

    std::uniform_int_distribution<> distribPos(0, posVariance);
    std::uniform_int_distribution<> distribVel(0, velVariance);
    std::uniform_int_distribution<> distribOrient(-orientVariance, orientVariance);
    int ballPosX = (distribPos(gen) + 2000) * dir;
    int ballVelX = (distribVel(gen) + 200) * dir;

    int carPosX = (distribPos(gen) + 300) * dir * -1 + ballPosX;
    int carVelX = distribVel(gen) * dir;

    float carYaw = (0.9 + distribOrient(gen)) * M_PI;
    carYaw = dir == -1 ? carYaw : M_PI - carYaw;

    bs.pos = Vec(ballPosX, -3000, RLGSC::CommonValues::BALL_RADIUS);
    bs.vel = Vec(ballVelX, 100, 10);
    bs.angVel = Vec(0, 0, 0);

    for (Car* car : arena->_cars) {
        CarState cs = {};
        cs.pos = Vec(carPosX, -3200, 20);
        cs.vel = Vec(carVelX, 100, 10);
        cs.angVel = Vec(0, 0, 0);
        Angle angle = Angle(carYaw, 0, 0);
        cs.rotMat = angle.ToRotMat();
        cs.boost = 100;
        car->SetState(cs);
    }

    arena->ball->SetState(bs);

    return RLGSC::GameState(arena);
}

RLGSC::GameState WeightedSampleSetter::ResetState(Arena* arena)
{
    //Commented while multi gamemodes are disabled
    //this->BuildArenaConfig(arena);

    float weightSum = 0;

    for (std::pair p : setters) {
        weightSum += p.second;
    }

    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> distrib(0, weightSum);

    float pick = distrib(gen);

    float current = 0;

    for (std::pair p : setters) {
        current += p.second;
        if (pick > current) {
            continue;
        }
        
        //Chosen setter will be here
        p.first->ResetState(arena);
        break;
    }

    return RLGSC::GameState(arena);
}

void WeightedSampleSetter::BuildArenaConfig(Arena* arena)
{
    //Based on gm probs, choose gamemode
    float maxTeamSize = gmProbs.size();

    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(1, maxTeamSize);

    int teamSize = distrib(gen);

    int nBlue = 0, nOrange = 0;
    for (Car* c : arena->GetCars()) {
        if (c->team == Team::BLUE) {
            nBlue++;
        }
        else {
            nOrange++;
        }
    }

    int blueToSpawn = teamSize - nBlue > 0 ? teamSize - nBlue : 0;
    int orangeToSpawn = teamSize - nOrange > 0 ? teamSize - nOrange : 0;

    for (int i = 0; i < blueToSpawn; i++) {
        arena->AddCar(Team::BLUE);
    }

    for (int i = 0; i < orangeToSpawn; i++) {
        arena->AddCar(Team::ORANGE);
    }

    int blueToRemove = teamSize - nBlue < 0 ? nBlue - teamSize : 0;
    int orangeToRemove = teamSize - nOrange < 0 ? nOrange - teamSize : 0;

    for (int i = 0; i < blueToRemove; i++) {
        for (Car* c : arena->GetCars()) {
            if (c->team == Team::BLUE) {
                arena->RemoveCar(c);
            }
        }
    }

    for (int i = 0; i < orangeToRemove; i++) {
        for (Car* c : arena->GetCars()) {
            if (c->team == Team::ORANGE) {
                arena->RemoveCar(c);
            }
        }
    }
}
