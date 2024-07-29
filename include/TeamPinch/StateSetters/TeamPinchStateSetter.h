#include <TeamPinch/TeamPinchUtils.h>
#include <RLGymSim_CPP/Utils/StateSetters/StateSetter.h>

using namespace RLGSC;

START_TEAMPINCH_STATES_NS

class TeamPinchStateSetter: public StateSetter{
public:
	TeamPinchStateSetter(){};
	virtual GameState ResetState(Arena* arena) override;
};

END_TEAMPINCH_NS
