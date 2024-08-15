#include <Recovery/RecoveryUtils.h>

using namespace RLGSC;

START_RECOVERY_REWARDS_NS

class RecoveryReward: public LoggableReward{
public:
	struct RecoveryArgs {
		/// <summary>
		/// Distance to ball weight
		/// </summary>
		float distanceToBallWeight = 1.0f;

		/// <summary>
		/// Flip time weight
		/// </summary>
		float flipTimeWeight = 1.0f;

		/// <summary>
		/// Velocity weight
		/// </summary>
		float velocityWeight = 1.0f;

		/// <summary>
		/// Double jump weight
		/// </summary>
		float doubleJumpWeight = 1.0f;
	};

	RecoveryReward(RecoveryArgs args = {}, std::string name = "Recovery") : LoggableReward(name), config(args) {};
	virtual float GetReward(const PlayerData& player, const GameState& state, const Action& prevAction) override;
private:
	RecoveryArgs config;
};

END_RECOVERY_NS
