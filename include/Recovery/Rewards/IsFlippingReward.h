#include <Recovery/RecoveryUtils.h>

START_RECOVERY_REWARDS_NS

class IsFlippingReward: public LoggableReward {
public:
	IsFlippingReward(std::string name = "Is flipping reward") : LoggableReward(name) {};
	virtual float GetReward(const PlayerData& player, const GameState& state, const Action& prevAction);
};

END_RECOVERY_NS