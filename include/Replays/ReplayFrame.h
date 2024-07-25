/*****************************************************************//**
 * \file   ReplayFrame.h
 * \brief  File containing all the classes definition for replay handling
 * 
 * \author Mathieu Suchet
 * \date   July 2024
 *********************************************************************/

#pragma once

#include <RLGymSim_CPP/Utils/BasicTypes/Lists.h>
#include <RLGymSim_CPP/Utils/Gamestates/GameState.h>
#include <RLGymSim_CPP/Utils/Gamestates/PhysObj.h>
#include <Replays/ReplayUtils.h>
#include "../../RLGymPPO_CPP/libsrc/json/nlohmann/json.hpp"
using json = nlohmann::json;

namespace RocketSim {
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Vec, x, y, z, _w)
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(RotMat, forward, right, up)
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PhysState, pos, vel, angVel, rotMat)
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CarControls, throttle, steer, pitch, yaw, roll, boost, jump, handbrake)
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(BallState, BALLSTATE_SERIALIZATION_FIELDS)
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CarState, CARSTATE_SERIALIZATION_FIELDS)
};

namespace RLGSC {
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ScoreLine, teamGoals)
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PhysObj, pos, vel, angVel, rotMat)
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PlayerData, carId, team, phys, physInv, carState, matchGoals, matchSaves, matchAssists, matchShots, matchShotPasses, matchBumps, matchDemos, boostPickups, hasJump, hasFlip, boostFraction, ballTouchedStep, ballTouchedTick)
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GameState, deltaTime, scoreLine, lastTouchCarID, ballState, ball, ballInv, boostPads, boostPadsInv, boostPadTimers, boostPadTimersInv, lastTickCount)
}

START_REPLAY_NS

/// <summary>
/// Represents the physical part of an object in a frame
/// </summary>
struct PhysicsFrame {
	Vec pos = Vec(), vel = Vec(), rot = Vec(), angVel = Vec();
	bool isSleeping = true;

	static PhysState ToPhysState(const PhysicsFrame& physObj);
};

/// <summary>
/// Represents the ball part of the frame
/// </summary>
struct BallFrame: PhysicsFrame {
	int hitTeamNum = -1;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(BallFrame, pos, vel, rot, angVel, isSleeping, hitTeamNum)

	static BallState ToBallState(const BallFrame& ballFrame);
};

/// <summary>
/// Represents the "game" part of the frame, mainly containing field data
/// </summary>
struct GameFrame {
	float time = 0, delta = 0, secondsRemaining = 0;
	int replicatedGameStateTimeRemaining = 0;
	bool isOvertime = false , ballHasBeenHit = false;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(GameFrame, time, delta, secondsRemaining, replicatedGameStateTimeRemaining, isOvertime, ballHasBeenHit)
};

/// <summary>
/// Represents a single player's data of the frame
/// </summary>
struct PlayerFrame: PhysicsFrame {
	Vec dodgeJumpTorque = Vec(), dodgeTorque = Vec();
	int throttle = 0, steer = 0, matchScore = 0, matchGoals = 0, matchAssists = 0, matchSaves = 0, matchShots = 0, team = 0, boostPickup = 0;
	bool handbrake = false, isBoostActive = false, isJumpActive = false, isDoubleJumpActive = false, isFlipCarActive = false, isDodgeActive = false;
	float boostAmount = 0.0f;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(PlayerFrame, pos, vel, angVel, rot, isSleeping, dodgeJumpTorque, dodgeTorque, throttle, steer, matchScore, matchAssists, matchSaves, matchShots, team, boostPickup, handbrake, isBoostActive, isJumpActive, isDoubleJumpActive, isFlipCarActive, isDodgeActive, boostAmount)

	static RLGSC::PlayerData ToPlayerData(const PlayerFrame& playerFrame);
};

/// <summary>
/// A frame of a replay
/// </summary>
class ReplayFrame {
public:
	BallFrame ball;
	GameFrame game;

	std::vector<PlayerFrame> players;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(ReplayFrame, ball, game, players)
};

/// <summary>
/// A frame where a goal occurred
/// </summary>
struct GoalFrame {
	int frame;
	Team team;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(GoalFrame, frame, team)
};

/// <summary>
/// The replay metadata, containing misc data about the replay
/// </summary>
struct ReplayMetadata {
	int numberOfFrames, numberOfPlayableFrames;
	int nBlue = 0, nOrange = 0;
	RLGSC::ScoreLine scoreLine;

	std::vector<GoalFrame> goalFrames;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(ReplayMetadata, numberOfFrames, numberOfPlayableFrames, scoreLine, goalFrames, nBlue, nOrange)
};

/// <summary>
/// Represents an actual gameplay period, used to eliminate goal celebrations
/// </summary>
class GameplayPeriod {
public:

	GameplayPeriod(int startFrame = 0, int endFrame = 0, int firstHitFrame = 0, int goalFrame = 0);
	int startFrame, endFrame, firstHitFrame, goalFrame;

	bool IsFrameInGameplayPeriod(int frameInd, int endDelay) const;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(GameplayPeriod, startFrame, endFrame, firstHitFrame, goalFrame)
};

/// <summary>
/// Analysis of the replay, contains gameplay periods
/// </summary>
struct ReplayAnalysis {
public:

	ReplayAnalysis(int endDelay = 0) : endDelay(endDelay) {};

	std::vector<GameplayPeriod> gameplayPeriods = {};

	int endDelay;

	bool IsFrameInAnyPeriod(int frameInd) const;
	int GetNumberOfPlayableFrames() const;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(ReplayAnalysis, gameplayPeriods)
};


/// <summary>
/// The replay resulting from the finished parsing of the .replay file
/// </summary>
class ConvertedReplay {
public:

	ReplayMetadata metadata;
	ReplayAnalysis analysis;
	std::vector<ReplayFrame> frames = {};

	void AddFrame(ReplayFrame frame) { this->frames.push_back(frame); }

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(ConvertedReplay, metadata, analysis, frames)
};

/// <summary>
/// The replay resulting from the interpolation of a converted replay
/// </summary>
class Replay {
public:
	ReplayMetadata metadata;
	ReplayAnalysis analysis;
	std::vector<RLGSC::GameState> states = {};

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Replay, metadata, analysis, states)
};

END_REPLAY_NS
