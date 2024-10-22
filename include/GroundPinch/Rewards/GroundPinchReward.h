#include <GroundPinch/GroundPinchUtils.h>
#include <Logging/LoggableReward.h>
#include <Pinch/Rewards/PinchReward.h>

using namespace RLGSC;

USE_LOGGING_NS;
USE_PINCH_REWARDS_NS;
START_GROUNDPINCH_REWARDS_NS

class GroundPinchReward: public LoggableReward{
public:
	struct PinchGroundSetupArgs {
		PinchReward::PinchArgs pinchRewardConfig = {};
	};

	GroundPinchReward(PinchGroundSetupArgs args, std::string name = "Ground pinch reward") : config(args), LoggableReward(name), pinchReward(PinchReward(args.pinchRewardConfig)) {};
	virtual void Reset(const RLGSC::GameState& initialState);
	virtual float GetReward(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction);
	virtual void LogAll(Report& report, bool final, std::string name = "", float weight = 1.f);


private:
	PinchGroundSetupArgs config;
	PinchReward pinchReward;
};

END_GROUNDPINCH_NS
