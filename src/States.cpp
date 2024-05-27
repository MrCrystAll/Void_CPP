#include "States.h"

RLGSC::GameState WallPinchSetter::ResetState(Arena* arena)
{
    BallState bs = {};

    bs.pos = Vec(-2000, -3000, RLGSC::CommonValues::BALL_RADIUS);
    bs.vel = Vec(-1400, 400, 10);
    bs.angVel = Vec(0, 0, 0);

    arena->ball->SetState(bs);

    for (Car* car : arena->_cars) {
        CarState cs = {};
        cs.pos = Vec(-1600, -3200, 20);
        cs.vel = Vec(-700, 400, 10);
        cs.angVel = Vec(0, 0, 0);
        Angle angle = Angle(0.90 * M_PI, 0, 0);
        cs.rotMat = angle.ToRotMat();
    }


    return RLGSC::GameState(arena);
}
