#pragma once
#include <RLGymSim_CPP/Utils/BasicTypes/Lists.h>
#include <RLGymPPO_CPP/Util/Report.h>
#include <LoggerUtils.h>

struct Metric {
	std::string name;
	bool isAvg = true;
};

//TODO: Optimize this shit to actually not loop every player for each logger and just do them all in one go

class Logger {
public:
	std::vector<Metric> metrics;

	virtual void Log(RLGPC::Report& report, RLGSC::GameState state);
	virtual void Reset() {}
protected:
	Logger(std::vector<Metric> metrics) : metrics(metrics) {};
	RLGSC::FList virtual GetMetrics(RLGSC::GameState state) { return {}; };
};

namespace BallLoggers {
	class BallSpeedLogger : public Logger {
	public:
		BallSpeedLogger() : Logger({ {.name = "ball_speed"}}) {};

	protected:
		RLGSC::FList virtual GetMetrics(RLGSC::GameState state) override;
	};


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

	class BallHeightLogger : public Logger {
	public:
		BallHeightLogger() : Logger({
			{.name = "ball_height"},
			}) {};
	protected:
		virtual RLGSC::FList GetMetrics(RLGSC::GameState state) override;
	};
}

namespace PlayerLoggers {
	class PlayerSpeedLogger : public Logger {
	public:
		PlayerSpeedLogger(): Logger({
			{.name = "player_speed"}
			}) {};
	protected:
		virtual RLGSC::FList GetMetrics(RLGSC::GameState state) override;
	};

	class PlayerBallTouchLogger : public Logger {
	public:
		PlayerBallTouchLogger(): Logger({
				{.name = "ball_touch_ratio"}
			}) {};
	protected:
		virtual RLGSC::FList GetMetrics(RLGSC::GameState state) override;
	};

	class PlayerInAirLogger : public Logger {
	public:
		PlayerInAirLogger(): Logger({
			{.name = "in_air_ratio"}
			}) {};
	protected:
		virtual RLGSC::FList GetMetrics(RLGSC::GameState state) override;
	};

	class PlayerHeightLogger : public Logger {
	public:
		PlayerHeightLogger() : Logger({
			{.name = "player_height"},
			}) {};
	protected:
		virtual RLGSC::FList GetMetrics(RLGSC::GameState state) override;
	};
}