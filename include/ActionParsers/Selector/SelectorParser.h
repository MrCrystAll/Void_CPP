#pragma once

#include <RLGymSim_CPP/Utils/ActionParsers/ActionParser.h>
#include <Utils/Selector/Submodel.h>
#include <Utils/Selector/SelectorChoice.h>

using namespace RLGSC;

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
