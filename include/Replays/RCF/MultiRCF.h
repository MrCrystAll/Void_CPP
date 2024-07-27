#pragma once

#include <Replays/RCF/AbstractRCF.h>

START_RCF_NS

class MultiRCF : public AbstractRCF {
protected:
	std::vector<AbstractRCF*> rcfs;

public:
	MultiRCF(std::vector<AbstractRCF*> rcfs) : rcfs(rcfs) {};
};

class AllRCF : public MultiRCF {
public:
	// Inherited via MultiRCF
	virtual bool PlayerFilter(const PlayerData player, const GameState state) override;
	virtual bool AllPlayersFilter(const GameState state);
};

class AnyRCF : public MultiRCF {
public:
	// Inherited via MultiRCF
	virtual bool PlayerFilter(const PlayerData player, const GameState state) override;
	virtual bool AllPlayersFilter(const GameState state);
};

END_RCF_NS