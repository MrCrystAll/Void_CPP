#include <Selector/SelectorUtils.h>
#include <RLGymSim_CPP/Utils/StateSetters/StateSetter.h>

using namespace RLGSC;

START_SELECTOR_STATES_NS

class SelectorStateSetter: public StateSetter{
public:
	SelectorStateSetter(){};
	virtual GameState ResetState(Arena* arena) override;
};

END_SELECTOR_NS
