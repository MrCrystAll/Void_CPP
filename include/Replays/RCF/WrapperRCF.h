#pragma once

#include <Replays/RCF/AbstractRCF.h>

START_RCF_NS

class WrapperRCF : public AbstractRCF {
protected:
	AbstractRCF* rcf;
public:
	virtual bool PlayerFilter(const PlayerData player, const GameState state);
	virtual bool AllPlayersFilter(const GameState state);
	WrapperRCF(AbstractRCF* rcf) : rcf(rcf) {};
};


class AllPlayersRCF : public WrapperRCF {
public:
	virtual bool AllPlayersFilter(const GameState state);
};


class AnyPlayerRCF : public WrapperRCF {
public:
	virtual bool AllPlayersFilter(const GameState state);
};

class OnePlayerRCF : public WrapperRCF {
protected:
	int playerId;
public:
	OnePlayerRCF(AbstractRCF* rcf, int playerId) : WrapperRCF(rcf), playerId(playerId) {};

	virtual bool AllPlayersFilter(const GameState state);
};

class NotRCF : public WrapperRCF {
	virtual bool AllPlayersFilter(const GameState state);
};

END_RCF_NS