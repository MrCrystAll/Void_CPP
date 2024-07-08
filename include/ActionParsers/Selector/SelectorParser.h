#pragma once

#include <RLGymSim_CPP/Utils/ActionParsers/ActionParser.h>
#include <Utils/Selector/Submodel.h>

using namespace RLGSC;

class SelectorParser: public ActionParser{
public:
	SelectorParser(std::vector<Submodel> submodels) : submodels(submodels) {};
private:
	std::vector<Submodel> submodels;


	// Inherited via ActionParser
	ActionSet ParseActions(const Input& actionsData, const GameState& gameState) override;

	int GetActionAmount() override;

};
