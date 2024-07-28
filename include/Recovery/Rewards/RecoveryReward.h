#include <Recovery/RecoveryUtils.h>

using namespace RLGSC;

START_RECOVERY_REWARDS_NS

class RecoveryReward: public LoggableReward{
public:
	RecoveryReward(std::string name = "Recovery"): LoggableReward(name){};
	virtual float GetReward(const PlayerData& player, const GameState& state, const Action& prevAction) override;
};

END_RECOVERY_NS
