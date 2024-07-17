/*****************************************************************//**
 * \file   ActionParser.h
 * \brief  Custom action parser
 * 
 * \author Mathieu Suchet
 * \date   July 2024
 *********************************************************************/

#pragma once

#include <RLGymSim_CPP/Utils/ActionParsers/DiscreteAction.h>
#include <Utils/VoidUtils.h>

START_ACTION_PARSER_NS

using namespace RLGSC;

class DashParser: public RLGSC::ActionParser
{
public:
	std::vector<Action> actions;
	DashParser();

	virtual ActionSet ParseActions(const Input& actionsData, const GameState& gameState) {
		ActionSet result;
		for (int idx : actionsData)
			result.push_back(actions[idx]);
		return result;
	}

	virtual int GetActionAmount() {
		return actions.size();
	}
};

END_ACTION_PARSER_NS