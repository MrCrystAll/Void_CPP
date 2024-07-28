#include <Recovery/RecoveryUtils.h>
#include <RLGymSim_CPP/Utils/StateSetters/StateSetter.h>

using namespace RLGSC;

START_RECOVERY_STATES_NS

class RecoveryStateSetter: public StateSetter{
public:
	RecoveryStateSetter(){};
	virtual GameState ResetState(Arena* arena) override;
};

END_RECOVERY_NS
