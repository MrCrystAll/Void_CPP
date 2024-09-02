#include <RLGymSim_CPP/Utils/OBSBuilders/DefaultOBS.h>
#include "ObsBuilder.h"
#include <Utils/LoggerUtils.h>

USE_OBS_BUILDER_NS;

using namespace RLGSC;

RLGSC::FList OnesObs::BuildOBS(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction)
{
    RLGSC::FList result = {};
    for (int i = 0; i < 70; i++) {
        result.push_back(1);
    }

    return result;
};

FList LockedDefaultObs::BuildOBS(const PlayerData& player, const GameState& state, const Action& prevAction)
{
	FList result = {};

	bool inv = player.team == Team::ORANGE;

	auto& ball = state.GetBallPhys(inv);
	auto& pads = state.GetBoostPads(inv);

	result += ball.pos * posCoef;
	result += ball.vel * velCoef;
	result += ball.angVel * angVelCoef;

	for (int i = 0; i < prevAction.ELEM_AMOUNT; i++)
		result += prevAction[i];

	for (int i = 0; i < CommonValues::BOOST_LOCATIONS_AMOUNT; i++)
		result += (float)pads[i];

	AddPlayerToOBS(result, player, inv);

	FList teammates = {}, opponents = {};

	int nBlue = 0, nOrange = 0;

	for (auto& otherPlayer : state.players) {
		if (otherPlayer.carId == player.carId)
			continue;

		if (otherPlayer.team == Team::BLUE and nBlue == teamSize - 1) continue;
		if (otherPlayer.team == Team::ORANGE and (nOrange == teamSize or not spawnOpponents)) continue;

		AddPlayerToOBS(
			(otherPlayer.team == player.team) ? teammates : opponents,
			otherPlayer,
			inv
		);

		if (otherPlayer.team == Team::BLUE) nBlue++;
		else nOrange++;
	}

	result += teammates;
	result += opponents;
	return result;
}
