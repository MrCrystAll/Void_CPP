/*****************************************************************//**
 * \file   CommonRCFs.h
 * \brief  A few basic RCF that can be used everywhere
 * 
 * \author Mathieu Suchet
 * \date   July 2024
 *********************************************************************/

#pragma once

#include <Replays/RCF/AbstractRCF.h>

START_RCF_NS

/// <summary>
/// Checks if any player is on the ground
/// </summary>
class OnGroundRCF : public AbstractRCF {
public:
	virtual bool PlayerFilter(const PlayerData player, const GameState state) override;
};

/// <summary>
/// Checks if any player is on the wall
/// </summary>
class OnWallRCF : public AbstractRCF {
private:
	int wallHeightTolerance;
	int wallWidthTolerance;
public:
	OnWallRCF(int wallHeightTolerance, int wallWidthTolerance) : wallHeightTolerance(wallHeightTolerance), wallWidthTolerance(wallWidthTolerance) {};
	virtual bool PlayerFilter(const PlayerData player, const GameState state) override;
};

/// <summary>
/// Checks if any player is on the ceiling
/// </summary>
class OnCeilingRCF : public AbstractRCF {
private:
	int wallHeightTolerance;
	int wallWidthTolerance;
public:
	OnCeilingRCF(int wallHeightTolerance, int wallWidthTolerance) : wallHeightTolerance(wallHeightTolerance), wallWidthTolerance(wallWidthTolerance) {};
	virtual bool PlayerFilter(const PlayerData player, const GameState state) override;
};

/// <summary>
/// Checks if any player is supersonic
/// </summary>
class SupersonicRCF : public AbstractRCF {
public:
	virtual bool PlayerFilter(const PlayerData player, const GameState state) override;
};

/// <summary>
/// Dribble situations
/// </summary>
class DribbleRCF : public AbstractRCF {
public:
	virtual bool PlayerFilter(const PlayerData player, const GameState state) override;
};

class ShootingRCF : public AbstractQueuedRCF {
public:
	ShootingRCF(int queueSize = 100) : AbstractQueuedRCF(queueSize) {};
	std::map<int, int> lastMatchShots = {};
	virtual bool PlayerFilter(const PlayerData player, const GameState state) override;
};

class SavingRCF : public AbstractQueuedRCF {
public:
	SavingRCF(int queueSize = 100) : AbstractQueuedRCF(queueSize) {};
	std::map<int, int> lastMatchSaves = {};
	virtual bool PlayerFilter(const PlayerData player, const GameState state) override;
};

class FlipResetRCF : public AbstractQueuedRCF {
public:
	FlipResetRCF(int queueSize = 50) : AbstractQueuedRCF(queueSize) {};
	virtual bool PlayerFilter(const PlayerData player, const GameState state) override;
};

END_RCF_NS