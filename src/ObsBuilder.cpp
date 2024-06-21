#include "ObsBuilder.h"

RLGSC::FList OnesObs::BuildOBS(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction)
{
    RLGSC::FList result = {};
    for (int i = 0; i < 70; i++) {
        result.push_back(1);
    }

    return result;
};

void RLGSC::DefaultOBS::AddPlayerToOBS(FList& obs, const PlayerData& player, bool inv) {
	PhysObj phys = player.GetPhys(inv);

	obs += phys.pos * posCoef;
	obs += phys.rotMat.forward;
	obs += phys.rotMat.up;
	obs += phys.vel * velCoef;
	obs += phys.angVel * angVelCoef;

	obs += {
		player.boostFraction,
			(float)player.carState.isOnGround,
			(float)player.hasFlip,
			(float)player.carState.isDemoed,
	};
}

RLGSC::FList RLGSC::DefaultOBS::BuildOBS(const PlayerData& player, const GameState& state, const Action& prevAction) {
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

	for (auto& otherPlayer : state.players) {
		if (otherPlayer.carId == player.carId)
			continue;

		AddPlayerToOBS(
			(otherPlayer.team == player.team) ? teammates : opponents,
			otherPlayer,
			inv
		);
	}

	result += teammates;
	result += opponents;

	return result;
}