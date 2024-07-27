/*****************************************************************//**
 * \file   WrapperRCF.h
 * \brief  A bunch of logic that can be applied on a single RCF
 * 
 * \author Mathieu Suchet
 * \date   July 2024
 *********************************************************************/

#pragma once

#include <Replays/RCF/AbstractRCF.h>

START_RCF_NS

/// <summary>
/// Applies a RCF on a state
/// </summary>
class WrapperRCF : public AbstractRCF {
protected:
	AbstractRCF* rcf;
public:
	virtual bool PlayerFilter(const PlayerData player, const GameState state);
	virtual bool AllPlayersFilter(const GameState state);
	WrapperRCF(AbstractRCF* rcf) : rcf(rcf) {};
};

/// <summary>
/// Checks if RCF matches all the players
/// </summary>
class AllPlayersRCF : public WrapperRCF {
public:
	virtual bool AllPlayersFilter(const GameState state);
};

/// <summary>
/// Checks if RCF matches any player
/// </summary>
class AnyPlayerRCF : public WrapperRCF {
public:
	virtual bool AllPlayersFilter(const GameState state);
};

/// <summary>
/// Checks if RCF matches a given player
/// </summary>
class OnePlayerRCF : public WrapperRCF {
protected:
	int playerId;
public:
	OnePlayerRCF(AbstractRCF* rcf, int playerId) : WrapperRCF(rcf), playerId(playerId) {};

	virtual bool AllPlayersFilter(const GameState state);
};

/// <summary>
/// Negates the RCF output
/// </summary>
class NotRCF : public WrapperRCF {
	virtual bool AllPlayersFilter(const GameState state);
};

END_RCF_NS