#include <WallPinch/WallPinchUtils.h>
#include <RLGymSim_CPP/Utils/StateSetters/StateSetter.h>

using namespace RLGSC;

START_WALLPINCH_STATES_NS

class WallPinchSetter : public RLGSC::StateSetter {
public:
	WallPinchSetter(float posVariance = 400.0f, float velVariance = 200.0f, float orientVariance = 0.5f);
	virtual RLGSC::GameState ResetState(Arena* arena);

private:
	float posVariance;
	float orientVariance;
	float velVariance;
};

END_WALLPINCH_NS
