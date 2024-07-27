#pragma once

#include <Replays/RCF/AbstractRCF.h>

START_RCF_NS

class OnGroundRCF : public AbstractRCF {
public:
	virtual bool PlayerFilter(const PlayerData player, const GameState state) override;
};

class OnWallRCF : public AbstractRCF {
private:
	int wallHeightTolerance;
	int wallWidthTolerance;
public:
	OnWallRCF(int wallHeightTolerance, int wallWidthTolerance) : wallHeightTolerance(wallHeightTolerance), wallWidthTolerance(wallWidthTolerance) {};
	virtual bool PlayerFilter(const PlayerData player, const GameState state) override;
};

class OnCeilingRCF : public AbstractRCF {
private:
	int wallHeightTolerance;
	int wallWidthTolerance;
public:
	OnCeilingRCF(int wallHeightTolerance, int wallWidthTolerance) : wallHeightTolerance(wallHeightTolerance), wallWidthTolerance(wallWidthTolerance) {};
	virtual bool PlayerFilter(const PlayerData player, const GameState state) override;
};

class SupersonicRCF : public AbstractRCF {
public:
	virtual bool PlayerFilter(const PlayerData player, const GameState state) override;
};

END_RCF_NS