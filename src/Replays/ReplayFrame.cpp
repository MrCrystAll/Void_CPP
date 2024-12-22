#include <Replays/ReplayFrame.h>

USE_REPLAY_NS;

BallState BallFrame::ToBallState(const BallFrame& ballFrame)
{
	PhysState ps = PhysicsFrame::ToPhysState(ballFrame);
	BallState bs = {};
	bs.pos = ps.pos;
	bs.vel = ps.vel;
	bs.angVel = ps.angVel;
	bs.rotMat = ps.rotMat;

	return bs;
}

RLGSC::PlayerData PlayerFrame::ToPlayerData(const PlayerFrame& playerFrame, const PlayerFrameControls lastControls)
{
	RLGSC::PlayerData playerData = {};
	playerData.team = (Team)(playerFrame.team - 15);
	playerData.boostPickups = playerFrame.boostPickup;
	playerData.boostFraction = playerFrame.boostAmount / 100;

	PhysState ps = PlayerFrame::ToPhysState(playerFrame);
	CarState cs = {};

	cs.pos = ps.pos;
	cs.vel = ps.vel;
	cs.angVel = ps.angVel;
	cs.rotMat = ps.rotMat;

	cs.boost = playerFrame.boostAmount / 100;
	cs.isFlipping = playerFrame.isFlipCarActive;
	cs.isDemoed = playerFrame.isSleeping;
	cs.handbrakeVal = playerFrame.controls.handbrake;
	cs.isJumping = playerFrame.isJumpActive;

	cs.lastControls.jump = lastControls.jump;
	cs.lastControls.boost = lastControls.boost;
	cs.lastControls.handbrake = lastControls.handbrake;
	cs.lastControls.throttle = lastControls.throttle;
	cs.lastControls.steer = lastControls.steer;
	cs.lastControls.pitch = lastControls.pitch;
	cs.lastControls.yaw = lastControls.yaw;
	cs.lastControls.roll = lastControls.roll;
	
	playerData.matchSaves = playerFrame.matchSaves;
	playerData.matchShots = playerFrame.matchShots;
	playerData.matchGoals = playerFrame.matchGoals;
	playerData.matchAssists = playerFrame.matchAssists;

	playerData.carState = cs;

	return playerData;
}

RocketSim::CarState PlayerFrame::ToCarState(const PlayerFrame& playerFrame, const PlayerFrame& lastPlayerState, const RLGSC::PlayerData& lastPdata)
{
	PhysState ps = PlayerFrame::ToPhysState(playerFrame);
	CarState cs = {};

	cs.pos = ps.pos;
	cs.vel = ps.vel;
	cs.angVel = ps.angVel;
	cs.rotMat = ps.rotMat;

	cs.boost = playerFrame.boostAmount / 100;
	cs.isFlipping = playerFrame.isFlipCarActive;
	cs.isDemoed = playerFrame.isSleeping;
	cs.handbrakeVal = playerFrame.controls.handbrake;
	cs.isJumping = playerFrame.isJumpActive;

	PlayerFrameControls lastControls = lastPlayerState.controls;

	cs.lastControls = CarControls();
	cs.lastControls.jump = lastControls.jump;
	cs.lastControls.boost = lastControls.boost;
	cs.lastControls.handbrake = lastControls.handbrake;
	cs.lastControls.throttle = lastControls.throttle;
	cs.lastControls.steer = lastControls.steer;
	cs.lastControls.pitch = lastControls.pitch;
	cs.lastControls.yaw = lastControls.yaw;
	cs.lastControls.roll = lastControls.roll;

	return cs;
}

PhysState PhysicsFrame::ToPhysState(const PhysicsFrame& physObj)
{
	PhysState ps = {};
	ps.pos = physObj.pos;
	ps.vel = physObj.vel;
	ps.angVel = physObj.angVel;
	Angle angle = {};
	btQuaternion(physObj.rot.x, physObj.rot.y, physObj.rot.z, physObj.rot._w).getEulerZYX(angle.yaw, angle.pitch, angle.roll);
	angle.roll = -angle.roll;
	ps.rotMat = angle.ToRotMat();

	return ps;
}

GameplayPeriod::GameplayPeriod(int startFrame, int endFrame, int firstHitFrame, int goalFrame) : startFrame(startFrame), endFrame(endFrame), firstHitFrame(firstHitFrame), goalFrame(goalFrame) {};

bool GameplayPeriod::IsFrameInGameplayPeriod(int frameInd, int endDelay) const
{
	bool isAfterStart = frameInd >= this->startFrame;
	if (goalFrame == -1) {
		return isAfterStart and frameInd < this->endFrame - endDelay;
	}
	return isAfterStart and frameInd < this->goalFrame - endDelay;
}

bool ReplayAnalysis::IsFrameInAnyPeriod(int frameInd) const
{
	for (const GameplayPeriod gp : this->gameplayPeriods) {
		if (gp.IsFrameInGameplayPeriod(frameInd, endDelay)) return true;
	}
	return false;
}

int ReplayAnalysis::GetNumberOfPlayableFrames() const
{
	int sum = 0;
	for (const GameplayPeriod gp : this->gameplayPeriods) {
		if (gp.goalFrame == -1) {
			sum += gp.endFrame - endDelay - gp.startFrame;
		}
		else {
			sum += gp.goalFrame - endDelay - gp.startFrame;
		}
	}
	return sum;
}

ReplayMetadata::PlayerMetadata ReplayMetadata::GetPlayerWithId(std::string id)
{
	for (ReplayMetadata::PlayerMetadata p : bluePlayers) {
		if (p.unique_id == id) return p;
	}
	for (ReplayMetadata::PlayerMetadata p : orangePlayers) {
		if (p.unique_id == id) return p;
	}
	VOID_ERR("Player with id \"" << id << "\" doesn't exist in this replay");
}
