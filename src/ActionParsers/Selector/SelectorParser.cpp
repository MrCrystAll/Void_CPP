#include "ActionParsers/Selector/SelectorParser.h"
#include <Utils/LoggerUtils.h>
#include <map>
#include <algorithm>

USE_SELECTOR_NS;

ActionSet SelectorParser::ParseActions(const Input& actionsData, const GameState& gameState)
{
    RLGSC::ActionSet resultActions = {};
    std::map<int, RLGSC::ActionSet> allSubAct = {};
    std::vector<Submodel> submodelsPicked = {};

    if (this->prevActions.size() == 0) {
        for (int j = 0; j < actionsData.size(); j++) {
            this->prevActions.push_back(RLGSC::Action());
        }
    }

    int i = 0;
    for (int act : actionsData) {
        if (allSubAct.contains(act)) {
            Submodel m = this->choice->GetSubmodels()[act];
            resultActions.push_back(allSubAct[act][i]);
            submodelsPicked.push_back(m);
        }
        else {
            Submodel m = this->choice->GetSubmodels()[act];
            allSubAct[act] = m.Act(gameState, this->prevActions, true);
            resultActions.push_back(allSubAct[act][i]);
            submodelsPicked.push_back(m);
        }
    }

    this->choice->UpdateCurrentSubmodels(submodelsPicked);
    return resultActions;
}

int SelectorParser::GetActionAmount()
{
    return this->choice->GetSubmodels().size();
}
