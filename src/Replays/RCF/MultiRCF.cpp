#include <Replays/RCF/MultiRCF.h>

USE_RCF_NS;

bool AllRCF::PlayerFilter(const PlayerData player, const GameState state)
{
	for (AbstractRCF* rcf : this->rcfs) {
		if (not rcf->PlayerFilter(player, state)) return false;
	}
	return true;
}

bool AllRCF::AllPlayersFilter(const GameState state)
{
	for (AbstractRCF* rcf : this->rcfs) {
		if (not rcf->AllPlayersFilter(state)) return false;
	}
	return true;
}

bool AnyRCF::PlayerFilter(const PlayerData player, const GameState state)
{
	for (AbstractRCF* rcf : this->rcfs) {
		if (rcf->PlayerFilter(player, state)) return true;
	}
	return false;
}

bool AnyRCF::AllPlayersFilter(const GameState state)
{
	for (AbstractRCF* rcf : this->rcfs) {
		if (rcf->AllPlayersFilter(state)) return true;
	}
	return false;
}