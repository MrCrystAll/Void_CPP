#include <Recovery/ObsBuilders/DashObsBuilder.h>
#include <Replays/RCF/RCFUtils.h>

USE_RECOVERY_OB_NS;

void DashObsBuilder::AddPlayerToOBS(RLGSC::FList& obs, const RLGSC::PlayerData& player, bool inv)
{
	RLGSC::PhysObj phys = player.GetPhys(inv);

	obs += phys.pos * posCoef;
	obs += phys.rotMat.forward;
	obs += phys.rotMat.up;
	obs += phys.vel * velCoef;
	obs += phys.angVel * angVelCoef;

	obs += player.carState.airTime;
	obs += player.carState.airTimeSinceJump;
	obs += player.carState.flipTime;
	obs += player.carState.flipRelTorque;

	int numWheels = 0;
	for (bool b : player.carState.wheelsWithContact) {
		numWheels += b;
	}

	obs += {
		player.boostFraction,
			(float)player.carState.isOnGround,
			(float)player.hasFlip,
			(float)player.carState.isDemoed,
			(float)player.carState.hasFlipped,
			(float)player.carState.hasJumped,
			(float)player.carState.hasDoubleJumped,
			(float)player.carState.isFlipping,
			(float)numWheels
	};
}

void DashObsBuilder::PreStep(const GameState& state)
{
	this->delaySinceLastFlip = std::min(this->delaySinceLastFlip + 1, 300);
}

RLGSC::FList DashObsBuilder::BuildOBS(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction)
{
	RLGSC::FList result = {};

	bool inv = player.team == Team::ORANGE;

	auto& ball = state.GetBallPhys(inv);
	auto& pads = state.GetBoostPads(inv);

	result += ball.pos * posCoef;
	result += ball.vel * velCoef;
	result += ball.angVel * angVelCoef;

	for (int i = 0; i < prevAction.ELEM_AMOUNT; i++)
		result += prevAction[i];

	for (int i = 0; i < RLGSC::CommonValues::BOOST_LOCATIONS_AMOUNT; i++)
		result += (float)pads[i];

	RLGSC::FList selfOBS = {};
	AddPlayerToOBS(selfOBS, player, inv);
	result += selfOBS;
	int playerObsSize = selfOBS.size();

	RLGSC::FList2 teammates = {}, opponents = {};

	result += (player.carState.pos - ball.pos) * posCoef;
	result += player.carState.pos.Dist(ball.pos) / 5000;

	//Is jumping, basically
	result += player.carState.hasJumped and not (player.carState.hasFlipped or player.carState.hasDoubleJumped);
	bool isOnWall = Void::Replays::RCF::IsOnWall(player);
	bool isOnCeiling = Void::Replays::RCF::IsOnCeiling(player);
	bool isOnCorner = Void::Replays::RCF::isOnCorner(player);

	Vec up = Vec(0, 0, 1);

	result += isOnWall;
	result += isOnCeiling;
	result += isOnCorner;

	if (isOnWall) up = player.carState.pos.x < 0 ? Vec(1, 0, 0) : Vec(-1, 0, 0);
	if (isOnCeiling) up = -up;
	if (isOnCorner) {
		float directionFrom45Deg = std::sqrt(2.0f) / 2;
		if (player.carState.pos.x > 0) {
			if (player.carState.pos.y > 0) {
				up = Vec(-directionFrom45Deg, -directionFrom45Deg, 0);
			}
			else {
				up = Vec(directionFrom45Deg, -directionFrom45Deg, 0);
			}
		}
		else {
			if (player.carState.pos.y > 0) {
				up = Vec(-directionFrom45Deg, directionFrom45Deg, 0);
			}
			else {
				up = Vec(directionFrom45Deg, directionFrom45Deg, 0);
			}
		}
	}

	result += up;

	if (player.carState.hasFlipped and not player.carState.isFlipping) this->delaySinceLastFlip = 0;

	if (player.carState.hasJumped and not (player.carState.hasFlipped or player.carState.hasDoubleJumped)) this->delaySinceOnlyJump = std::min(300, this->delaySinceOnlyJump + 1);
	else this->delaySinceOnlyJump = 0;

	result += this->delaySinceLastFlip;
	result += this->delaySinceOnlyJump;

	for (auto& otherPlayer : state.players) {
		if (otherPlayer.carId == player.carId)
			continue;

		RLGSC::FList playerObs = {};
		AddPlayerToOBS(
			playerObs,
			otherPlayer,
			inv
		);

		((otherPlayer.team == player.team) ? teammates : opponents).push_back(playerObs);
	}

	if (teammates.size() > maxPlayers - 1)
		RG_ERR_CLOSE("DefaultOBSPadded: Too many teammates for OBS, maximum is " << (maxPlayers - 1));

	if (opponents.size() > maxPlayers)
		RG_ERR_CLOSE("DefaultOBSPadded: Too many opponents for OBS, maximum is " << maxPlayers);

	for (int i = 0; i < 2; i++) {
		auto& playerList = i ? teammates : opponents;
		int targetCount = i ? maxPlayers - 1 : maxPlayers;

		while (playerList.size() < targetCount) {
			RLGSC::FList pad = RLGSC::FList(playerObsSize);
			playerList.push_back(pad);
		}
	}

	// Shuffle both lists
	std::shuffle(teammates.begin(), teammates.end(), ::Math::GetRandEngine());
	std::shuffle(opponents.begin(), opponents.end(), ::Math::GetRandEngine());

	for (auto& teammate : teammates)
		result += teammate;
	for (auto& opponent : opponents)
		result += opponent;

	return result;
}
