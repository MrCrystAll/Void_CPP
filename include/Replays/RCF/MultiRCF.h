/*****************************************************************//**
 * \file   MultiRCF.h
 * \brief  Multi RCF Declarations
 * 
 * \author Mathieu Suchet
 * \date   July 2024
 *********************************************************************/

#pragma once

#include <Replays/RCF/AbstractRCF.h>

START_RCF_NS

/// <summary>
/// A multi RCF can contain multiple RCFs and will match the conditions based on the user logic (all, any)
/// </summary>
class MultiRCF : public AbstractRCF {
protected:
	std::vector<AbstractRCF*> rcfs;

public:
	MultiRCF(std::vector<AbstractRCF*> rcfs) : rcfs(rcfs) {};
};

/// <summary>
/// Checks if all the rcfs match
/// </summary>
class AllRCF : public MultiRCF {
public:
	// Inherited via MultiRCF
	virtual bool PlayerFilter(const PlayerData player, const GameState state) override;
	virtual bool AllPlayersFilter(const GameState state);
};

/// <summary>
/// Checks if any rcf matches
/// </summary>
class AnyRCF : public MultiRCF {
public:
	// Inherited via MultiRCF
	virtual bool PlayerFilter(const PlayerData player, const GameState state) override;
	virtual bool AllPlayersFilter(const GameState state);
};

END_RCF_NS