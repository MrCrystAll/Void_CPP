#include <Replays/RCF/WrapperRCF.h>

USE_RCF_NS;

bool WrapperRCF::PlayerFilter(const PlayerData player, const GameState state)
{
	return this->rcf->PlayerFilter(player, state);
}

bool WrapperRCF::AllPlayersFilter(const GameState state)
{
	return this->rcf->AllPlayersFilter(state);
}

bool AllPlayersRCF::AllPlayersFilter(const GameState state)
{
	for (const PlayerData p : state.players) {
		if (not this->PlayerFilter(p, state)) return false;
	}
	return true;
}

bool AnyPlayerRCF::AllPlayersFilter(const GameState state)
{
	for (const PlayerData p : state.players) {
		if (this->PlayerFilter(p, state)) return true;
	}
	return false;
}

bool OnePlayerRCF::AllPlayersFilter(const GameState state)
{
	for (const PlayerData p : state.players) {
		if (p.carId == this->playerId) return this->PlayerFilter(p, state);
	}

	//If you manage to come here, we return false since it means the player you want to detect is not in the state
	return false;
}

bool NotRCF::AllPlayersFilter(const GameState state)
{
	return not this->rcf->AllPlayersFilter(state);
}
