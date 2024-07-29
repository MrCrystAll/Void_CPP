#include <CeilingPinch/CeilingPinchUtils.h>
#include <RLGymSim_CPP/Utils/StateSetters/StateSetter.h>

using namespace RLGSC;

START_CEILINGPINCH_STATES_NS

class OverfittingCeilingPinchSetter : public RLGSC::StateSetter {
public:
	virtual RLGSC::GameState ResetState(Arena* arena);
};

class CeilingPinchSetter : public RLGSC::StateSetter {
public:
	CeilingPinchSetter(float posVariance = 400.0f, float velVariance = 200.0f, float orientVariance = 0.3f) : posVariance(posVariance), velVariance(velVariance), orientVariance(orientVariance) {};
	virtual RLGSC::GameState ResetState(Arena* arena);

private:
	float posVariance;
	float orientVariance;
	float velVariance;
};

END_CEILINGPINCH_NS
