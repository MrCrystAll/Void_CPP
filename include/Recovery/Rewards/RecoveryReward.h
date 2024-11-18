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

		/// <summary>
		/// Getting reset from dash weight
		/// </summary>
		float dashResultWeight = 1.0f;
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
	std::map<int, bool> gotFlipFromDash = {};

	std::map<int, bool> lastIsOnGround = {};
	std::map<int, bool> lastHasFlipped = {};
	std::map<int, float> lastLinVel = {};

	RLGSC::FaceBallReward* faceball = new FaceBallReward();

	/// <summary>
	/// Punishes double jump
	/// </summary>
	/// <param name="player">Player to reward</param>
	void AddDoubleJumpPunishment(const PlayerData& player);

	/// <summary>
	/// Dist to ball reward
	/// </summary>
	/// <param name="player">Player to reward</param>
	/// <param name="state">Current state</param>
	void DistToBall(const PlayerData& player, const GameState& state);

	/// <summary>
	/// Velocity reward under certain threshold of flip time
	/// </summary>
	/// <param name="player">Player to reward</param>
	void VelocityReward(const PlayerData& player);

	/// <summary>
	/// Facing up relative to the surface you're on
	/// </summary>
	/// <param name="player">Player to reward</param>
	void UpFacingReward(const PlayerData& player);

	/// <summary>
	/// Rewards short delays between flips
	/// </summary>
	/// <param name="player">Player to reward</param>
	void FlipDelayReward(const PlayerData& player);

	/// <summary>
	/// Rewards short flips
	/// </summary>
	/// <param name="player">Player to reward</param>
	void FlipTimeReward(const PlayerData& player);

	/// <summary>
	/// Punishes jump time too long
	/// </summary>
	/// <param name="player">Player to reward</param>
	void OnlyJumpHeldTooLongPunishment(const PlayerData& player);

	/// <summary>
	/// Punishes going too high
	/// </summary>
	/// <param name="player">Player to reward</param>
	void HeightLimitPunishment(const PlayerData& player);

	/// <summary>
	/// Rewards dash streaking
	/// </summary>
	/// <param name="player">Player to reward</param>
	void DashStreakReward(const PlayerData& player);

	/// <summary>
	/// Rewards dashes, sparse reward
	/// </summary>
	/// <param name="player">Player to reward</param>
	void DashReward(const PlayerData& player);

	/// <summary>
	/// Rewards having a flip from dashing
	/// </summary>
	/// <param name="player">Player to reward</param>
	void DashResultReward(const PlayerData& player);
};

END_RECOVERY_NS
