#include <Recovery/RecoveryUtils.h>
#include <Logging/LoggableReward.h>
#include <RLGymSim_CPP/Utils/RewardFunctions/CommonRewards.h>

using namespace RLGSC;

USE_LOGGING_NS;
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

		/// <summary>
		/// Facing up weight
		/// </summary>
		float facingUpWeight = 2.0f;
	};

	RecoveryReward(RecoveryArgs args = {}, std::string name = "Recovery") : LoggableReward(name), config(args) {};
	virtual void PreStep(const GameState& state) override;
	virtual float GetReward(const PlayerData& player, const GameState& state, const Action& prevAction) override;
	virtual void Reset(const GameState& initialState) override;
private:
	RecoveryArgs config;

	std::map<int, int> delaySinceLastHasFlipped = {};
	std::map<int, int> delaySinceOnlyJump = {};
	std::map<int, int> dashStreaks = {};


	std::map<int, bool> lastIsOnGround = {};
	std::map<int, bool> lastHasFlipped = {};

	RLGSC::FaceBallReward* faceball = new FaceBallReward();

	void AddDoubleJumpPunishment(const PlayerData& player);
	void DistToBall(const PlayerData& player, const GameState& state);
	void VelocityReward(const PlayerData& player);
	void UpFacingReward(const PlayerData& player);
	void FlipDelayReward(const PlayerData& player);
	void FlipTimeReward(const PlayerData& player);
	void OnlyJumpHeldTooLongPunishment(const PlayerData& player);
	void HeightLimitPunishment(const PlayerData& player);
	void DashStreakReward(const PlayerData& player);
	void DashReward(const PlayerData& player);
};

END_RECOVERY_NS
