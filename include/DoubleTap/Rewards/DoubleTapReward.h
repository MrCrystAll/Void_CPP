#include <DoubleTap/DoubleTapUtils.h>
#include <Logging/LoggableReward.h>

using namespace RLGSC;

USE_LOGGING_NS;

START_DOUBLETAP_REWARDS_NS

/**
 * @brief Basic double tap reward
 */
	class DoubleTapReward : public LoggableReward {
	public:

		/**
		 * @brief Ball handling
		 */
		struct BallHandling {
			/**
			 * @brief Minimum distance to start going for aim
			 */
			float minDistToBall = 350.0f;

			/**
			 * @brief Reduction for ball dist to agent
			 */
			float distBallReduction = 50.0f;

			/**
			 * @brief Maximum distance to trigger touch from bb
			 */
			float maxDistFromBB = 500.0f;

			/**
			 * @brief Minimum height to trigger touch
			 */
			float minHeightToTrigger = RLGSC::CommonValues::GOAL_HEIGHT + RLGSC::CommonValues::BALL_RADIUS;

			/**
			 * @brief Touch weight
			 */
			float touchW = 10.0f;

			/**
			 * @brief Minimum similarity required to ball to trigger a reward
			 */
			float minSimilarityAgentBall = 0.7f;

			/**
			 * @brief Similarity agent ball weight
			 */
			float similarityAgentBallW = 10.0f;
		};

		struct GoalHandling {
			//Minimum similarity to trigger goal dir
			float minSimilarityGoalBall = 0.5f;

			//Weight for similarity goal ball dir
			float similarityGoalBallW = 10.0f;

			//Weight for goal
			float goalW = 100.0f;

			//Goal speed weight
			float goalSpeedW = 10.0f;

			//Concede weight
			float concedeW = 1.0f;
		};

		struct DoubleTapArgs {
			BallHandling ballHandling;
			GoalHandling goalHandling;
		};

		struct PhysObjLastHit {
			PlayerData physObj;
			int lastStepHit = 0;
		};

		struct PhysObjHandling {
			std::map<int, PhysObjLastHit> allCurrentHits;

			void Clear();
			void Reset(const RLGSC::GameState& initialState);
			void Update(const RLGSC::GameState& initialState, const RLGSC::PlayerData& player, const int nbSteps);
			const PlayerData* GetLatestHit(const Team team);
		};

		DoubleTapReward(std::string name, DoubleTapArgs args) : config(args), LoggableReward(name), scoreLine(RLGSC::ScoreLine()) {};
		virtual void Reset(const RLGSC::GameState& initialState);
		virtual RLGSC::FList GetAllRewards(const RLGSC::GameState& state, const RLGSC::ActionSet& prevActions, bool final) override;
		virtual float GetReward(const RLGSC::PlayerData& player, const RLGSC::GameState& state, const RLGSC::Action& prevAction);

	private:
		ScoreLine scoreLine;
		DoubleTapArgs config;
		PhysObjHandling physHandling;
		int nbSteps = 0;
};

/**
 * @brief A reward that uses double tap reward
 */
class UseDTReward : public LoggableReward {
public:
	UseDTReward(std::string name, DoubleTapReward::DoubleTapArgs dtConfig) : dtConfig(dtConfig), LoggableReward(name), dtReward(DoubleTapReward("DT", dtConfig)) {};
	virtual void Reset(const RLGSC::GameState& initialState) override;
	virtual void LogAll(Report& report, bool final, std::string name = "", float weight = 1.f);
	virtual void PreStep(const GameState& state);
	virtual std::vector<float> GetAllRewards(const GameState& state, const ActionSet& prevActions, bool final);
protected:
	DoubleTapReward::DoubleTapArgs dtConfig;
	DoubleTapReward dtReward;
};

END_DOUBLETAP_NS
