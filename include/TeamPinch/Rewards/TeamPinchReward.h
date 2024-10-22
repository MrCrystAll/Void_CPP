#include <TeamPinch/TeamPinchUtils.h>
#include <Logging/LoggableReward.h>
#include <Pinch/Rewards/PinchReward.h>

using namespace RLGSC;

USE_LOGGING_NS;
USE_PINCH_REWARDS_NS;
START_TEAMPINCH_REWARDS_NS

class TeamPinchReward: public LoggableReward{
public:
	struct PinchTeamSetupArgs {
		PinchReward::PinchArgs pinchRewardConfig = {};
	};

	TeamPinchReward(PinchTeamSetupArgs args, std::string name = "Team pinch reward") : config(args), LoggableReward(name), pinchReward(PinchReward(args.pinchRewardConfig)) {};
	virtual void Reset(const RLGSC::GameState& initialState);
	virtual float GetReward(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction);
	virtual void LogAll(Report& report, bool final, std::string name = "", float weight = 1.f);


private:
	PinchTeamSetupArgs config;
	PinchReward pinchReward;
};

END_TEAMPINCH_NS
