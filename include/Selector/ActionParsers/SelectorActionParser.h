#include <Selector/SelectorUtils.h>
#include <RLGymSim_CPP/Utils/ActionParsers/DiscreteAction.h>
#include <Selector/SelectorChoice.h>

using namespace RLGSC;

START_SELECTOR_AP_NS

/// <summary>
/// The parser used for the selector
/// </summary>
class SelectorParser : public ActionParser {
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
