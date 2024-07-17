#include <Logging/LoggableReward.h>
#include <Utils/DashesUtils.h>

START_REWARDS_DASHES_NS

class IsFlippingReward: public LoggableReward {
public:
	IsFlippingReward(std::string name = "Is flipping reward") : LoggableReward(name) {};
	virtual float GetReward(const PlayerData& player, const GameState& state, const Action& prevAction);
};

END_REWARDS_NS