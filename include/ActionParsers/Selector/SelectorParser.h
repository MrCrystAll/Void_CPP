/*****************************************************************//**
 * \file   SelectorParser.h
 * \brief  The parser used for the selector
 * 
 * \author Mathieu Suchet
 * \date   July 2024
 *********************************************************************/

#pragma once

#include <RLGymSim_CPP/Utils/ActionParsers/ActionParser.h>
#include <Utils/Selector/Submodel.h>
#include <Utils/Selector/SelectorChoice.h>
#include <Utils/Selector/SelectorUtils.h>

START_SELECTOR_NS

using namespace RLGSC;

/// <summary>
/// The parser used for the selector
/// </summary>
class SelectorParser: public ActionParser{
public:
	SelectorParser(SelectorChoice* choice) : prevActions({}), choice(choice) {};
private:
	RLGSC::ActionSet prevActions;

	// Inherited via ActionParser
	ActionSet ParseActions(const Input& actionsData, const GameState& gameState) override;

	int GetActionAmount() override;
	SelectorChoice* choice;

};

END_SELECTOR_NS