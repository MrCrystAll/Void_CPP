#include "States/DoubleTap/GroundDoubleTapState.h"


GameState GroundDoubleTapState::ResetState(Arena* arena)
{

    BallState bs = {};
    bs.pos = { 300, 2000, CommonValues::BALL_RADIUS };
    bs.vel = {-300, -1200, 0};

    arena->ball->SetState(bs);

    for (Car* c : arena->GetCars()) {
        CarState cs = {};
        if (c->team == Team::BLUE) {
            cs.pos = { 0, -1000, 17 };
            cs.vel = { 0, 1000, 0 };
            cs.rotMat = Angle(M_PI_2).ToRotMat();
        }
        else {
            cs.pos = { 0, CommonValues::BACK_WALL_Y, 17 };
            cs.rotMat = Angle(-M_PI_2).ToRotMat();
        }

        c->SetState(cs);
    }
    
    return GameState(arena);
}
