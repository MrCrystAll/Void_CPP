#include "ActionParsers/Selector/SelectorParser.h"

ActionSet SelectorParser::ParseActions(const Input& actionsData, const GameState& gameState)
{
    return ActionSet();
}

int SelectorParser::GetActionAmount()
{
    return this->submodels.size();
}
