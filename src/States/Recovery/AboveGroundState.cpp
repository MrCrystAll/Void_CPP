#include "States/Recovery/AboveGroundState.h"

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
			cs.pos = { borderLeft + stepX * iBlue, -4000, 17 };
			cs.rotMat = Angle(M_PI_2, 0, M_PI_4).ToRotMat();

			iBlue++;
		}
		else {
			cs.pos = { borderLeft + stepX * iOrange, 4000, 17 };
			cs.rotMat = Angle(-M_PI_2, 0, M_PI_4).ToRotMat();

			iOrange++;
		}
		cs.boost = 0;

		c->SetState(cs);
	}

	return RLGSC::GameState(arena);
}

GameState RandomRecoveryState::ResetState(Arena* arena)
{
	randomState.ResetState(arena);
	for (Car* c : arena->GetCars()) {
		CarState cs = c->GetState();

		cs.boost = 0;

		c->SetState(cs);
	}
	return GameState(arena);
}
