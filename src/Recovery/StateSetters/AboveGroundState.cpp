#include <Recovery/StateSetters/AboveGroundState.h>

USE_RECOVERY_STATES_NS;

GameState OverfittingGroundRecoveryState::ResetState(Arena* arena)
{
	BallState bs = {};
	bs.pos = { -200, 0, 93 };

	arena->ball->SetState(bs);

	float stepX = (CommonValues::SIDE_WALL_X * 2 - 1552 * 2) / 3;
	int borderLeft = -CommonValues::SIDE_WALL_X + 1152;

	int iBlue = 0;
	int iOrange = 0;

	for (Car* c : arena->GetCars()) {
		CarState cs = {};

		if (c->team == Team::BLUE) {
			cs.pos = { borderLeft + stepX * iBlue, -4000, 90 };
			cs.rotMat = Angle(M_PI_2, 0, M_PI_4).ToRotMat();

			iBlue++;
		}
		else {
			cs.pos = { borderLeft + stepX * iOrange, 4000, 90 };
			cs.rotMat = Angle(-M_PI_2, 0, M_PI_4).ToRotMat();

			iOrange++;
		}
		cs.boost = 0;
		cs.isFlipping = false;
		cs.hasFlipped = true;
		cs.hasJumped = true;

		c->SetState(cs);
	}

	return RLGSC::GameState(arena);
}

GameState RandomRecoveryState::ResetState(Arena* arena)
{

	BallState bs = {};
	bs.pos = Vec(Math::RandFloat(-CommonValues::SIDE_WALL_X + 1152, CommonValues::SIDE_WALL_X - 1152), Math::RandFloat(-CommonValues::BACK_WALL_Y + 1152, CommonValues::BACK_WALL_Y - 1152), Math::RandFloat(60, 200));
	bs.vel = Vec(Math::RandFloat(-1100, 1100), Math::RandFloat(-1100, 1100), Math::RandFloat(-600, 600));

	arena->ball->SetState(bs);

	for (Car* c : arena->GetCars()) {
		CarState cs = c->GetState();

		cs.pos = Vec(Math::RandFloat(-CommonValues::SIDE_WALL_X + 1152, CommonValues::SIDE_WALL_X - 1152), Math::RandFloat(-CommonValues::BACK_WALL_Y + 1152, CommonValues::BACK_WALL_Y - 1152), Math::RandFloat(60, 200));
		cs.vel = Vec(Math::RandFloat(-1100, 1100), Math::RandFloat(-1100, 1100), Math::RandFloat(-600, 600));
		cs.angVel = Vec(Math::RandFloat(-M_PI_2, M_PI_2), Math::RandFloat(-M_PI_2, M_PI_2), Math::RandFloat(-M_PI_2, M_PI_2));
		cs.rotMat = Angle(Math::RandFloat(-M_PI_2, M_PI_2), Math::RandFloat(-M_PI_2, M_PI_2), Math::RandFloat(-M_PI_2, M_PI_2)).ToRotMat();


		cs.hasFlipped = true;
		cs.hasJumped = true;

		cs.boost = 0;

		c->SetState(cs);
	}
	return GameState(arena);
}
