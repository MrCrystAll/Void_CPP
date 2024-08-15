/*****************************************************************//**
 * \file   Loggers.h
 * \brief  All the loggers to track metrics
 * 
 * \author Mathieu Suchet
 * \date   July 2024
 *********************************************************************/

#pragma once
#include <RLGymSim_CPP/Utils/BasicTypes/Lists.h>
#include <RLGymPPO_CPP/Util/Report.h>
#include <Utils/VoidUtils.h>

START_LOGGERS_NS

/// <summary>
/// A structure to hold a name and a bool to check if average
/// </summary>
struct Metric {
	std::string name;
	bool isAvg = true;
};

//TODO: Optimize this shit to actually not loop every player for each logger and just do them all in one go



/**
 * @brief Base logger class
 */
class Logger {
public:

	/**
	 * @brief All the metrics to pick 
	 */
	std::vector<Metric> metrics;

	/**
	 * @brief Log to report
	 * 
	 * @param report The report to log to
	 * @param state The state to take metric from
	 */
	virtual void Log(RLGPC::Report& report, RLGSC::GameState state);

	/**
	 * @brief Reset if necessary
	 */
	virtual void Reset() {}
protected:
	Logger(std::vector<Metric> metrics) : metrics(metrics) {};

	/**
	 * @brief Method that returns all the metrics from the state
	 * 
	 * @param state The state used for the metrics
	 */
	RLGSC::FList virtual GetMetrics(RLGSC::GameState state) { return {}; };
};

/**
 * @brief Ball loggers
 */
namespace BallLoggers {
	/**
	 * @brief Logger that tracks ball speed
	 */
	class BallSpeedLogger : public Logger {
	public:
		BallSpeedLogger() : Logger({ {.name = "ball_speed"}}) {};

	protected:
		RLGSC::FList virtual GetMetrics(RLGSC::GameState state) override;
	};

	/**
	 * @brief Logger that tracks ball velocity
	 */
	class BallVelocityLogger : public Logger {
	public:
		BallVelocityLogger() : Logger({
			{.name = "ball_vel_x"},
			{.name = "ball_vel_y"},
			{.name = "ball_vel_z"},
			}) {};
	protected:
		virtual RLGSC::FList GetMetrics(RLGSC::GameState state) override;
	};

	/**
	 * @brief Logger that tracks ball position
	 */
	class BallPositionLogger : public Logger {
	public:
		BallPositionLogger() : Logger({
			{.name = "ball_pos_x"},
			{.name = "ball_pos_y"},
			{.name = "ball_pos_z"},
			}) {};
	protected:
		virtual RLGSC::FList GetMetrics(RLGSC::GameState state) override;
	};
	
	/**
	 * @brief Logger that tracks ball height
	 */
	class BallHeightLogger : public Logger {
	public:
		BallHeightLogger() : Logger({
			{.name = "ball_height"},
			}) {};
	protected:
		virtual RLGSC::FList GetMetrics(RLGSC::GameState state) override;
	};
}

/**
 * @brief Player loggers
 */
namespace PlayerLoggers {

	/**
	 * @brief Logger that tracks all players average speed
	 */
	class PlayerSpeedLogger : public Logger {
	public:
		PlayerSpeedLogger(): Logger({
			{.name = "player_speed"}
			}) {};
	protected:
		virtual RLGSC::FList GetMetrics(RLGSC::GameState state) override;
	};

	/**
	 * @brief Logger that tracks player's average touch rate
	 */
	class PlayerBallTouchLogger : public Logger {
	public:
		PlayerBallTouchLogger(): Logger({
				{.name = "ball_touch_ratio"}
			}) {};
	protected:
		virtual RLGSC::FList GetMetrics(RLGSC::GameState state) override;
	};

	/**
	 * @brief Logger that tracks player's in air ratio
	 */
	class PlayerInAirLogger : public Logger {
	public:
		PlayerInAirLogger(): Logger({
			{.name = "in_air_ratio"}
			}) {};
	protected:
		virtual RLGSC::FList GetMetrics(RLGSC::GameState state) override;
	};

	/**
	 * @brief Logger that tracks player's height
	 */
	class PlayerHeightLogger : public Logger {
	public:
		PlayerHeightLogger() : Logger({
			{.name = "player_height"},
			}) {};
	protected:
		virtual RLGSC::FList GetMetrics(RLGSC::GameState state) override;
	};

	/**
	 * @brief Logger that tracks number of demos
	 */
	class DemoLogger : public Logger {
	public:
		DemoLogger() : Logger({ {.name = "demoes", .isAvg = false} }) {};
	protected:
		virtual RLGSC::FList GetMetrics(RLGSC::GameState state) override;
	};

	/**
	 * @brief Logger that tracks the average flip time
	 */
	class FlipTimeLogger : public Logger {
	public:
		FlipTimeLogger() : Logger({ {.name = "flip_time", .isAvg = true} }) {};
	protected:
		virtual RLGSC::FList GetMetrics(RLGSC::GameState state) override;
	};

	/**
	 * @brief Logger that tracks the average flip time
	 */
	class HasJumpedNotFlippedLogger : public Logger {
	public:
		HasJumpedNotFlippedLogger() : Logger({ {.name = "only_jump_time", .isAvg = true} }) {};
	protected:
		virtual RLGSC::FList GetMetrics(RLGSC::GameState state) override;
	};


}
END_LOGGERS_NS