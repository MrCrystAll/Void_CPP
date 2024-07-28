#include "ObsBuilders/Dashes/DashObsBuilder.h"

USE_OBS_BUILDER_NS;

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

	obs += {
		player.boostFraction,
			(float)player.carState.isOnGround,
			(float)player.hasFlip,
			(float)player.carState.isDemoed,
	};
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
