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
    int ballPosX = distribPos(gen) + 2000 * dir;
    int ballVelX = distribVel(gen) + 1400 * dir;

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
