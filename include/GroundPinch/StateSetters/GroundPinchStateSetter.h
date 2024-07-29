#include <GroundPinch/GroundPinchUtils.h>
#include <RLGymSim_CPP/Utils/StateSetters/StateSetter.h>

using namespace RLGSC;

START_GROUNDPINCH_STATES_NS

class GroundPinchStateSetter: public StateSetter{
public:
	GroundPinchStateSetter(){};
	virtual GameState ResetState(Arena* arena) override;
};

END_GROUNDPINCH_NS
