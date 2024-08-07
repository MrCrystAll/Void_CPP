#include <DoubleTap/DoubleTapUtils.h>
#include <RLGymSim_CPP/Utils/StateSetters/StateSetter.h>

using namespace RLGSC;

START_DOUBLETAP_STATES_NS

class OverfittingDoubleTapStateSetter : public RLGSC::StateSetter {
public:
	virtual RLGSC::GameState ResetState(Arena* arena);
};

class DoubleTapStateSetter: public StateSetter{
public:

	virtual RLGSC::GameState ResetState(Arena* arena);

	struct PhysObjVariance {
		Vec posVariance = Vec(400.0f, 0.0f, 200.0f);
		Vec velVariance = Vec(400.0f, 400.0f, 100.0f);
		Angle orientVariance = Angle(2.0f, 2.0f, 2.0f);
		Vec angVelVariance = Vec(100.0f, 100.0f, 100.0f);
	};

	struct DoubleTapStateArgs {
		bool bothSides = false;

		PhysObjVariance carVariance;
		PhysObjVariance ballVariance;

	};

	DoubleTapStateSetter(DoubleTapStateArgs args) : config(args) {};

private:
	DoubleTapStateArgs config;
};

END_DOUBLETAP_NS
