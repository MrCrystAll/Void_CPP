#include <Logging/LoggableReward.h>

class IsFlippingReward: public LoggableReward {
public:
	IsFlippingReward(std::string name = "Is flipping reward") : LoggableReward(name) {};
	virtual float GetReward(const PlayerData& player, const GameState& state, const Action& prevAction);
};
