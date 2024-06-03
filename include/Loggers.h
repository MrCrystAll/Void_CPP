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
	RLGSC::FList virtual GetMetrics(RLGSC::GameState state) = 0;
};

namespace BallLoggers {
	class BallSpeedLogger : public Logger {
	public:
		std::vector<Metric> metrics = {
			{.name = "ball_speed"}
		};
	protected:
		RLGSC::FList virtual GetMetrics(RLGSC::GameState state) override;
	};


	class BallVelocityLogger : public Logger {
	public:
		std::vector<Metric> metrics = {
			{.name = "ball_vel_x"},
			{.name = "ball_vel_y"},
			{.name = "ball_vel_z"},
		};
	protected:
		virtual RLGSC::FList GetMetrics(RLGSC::GameState state) override;
	};

	class BallPositionLogger : public Logger {
	public:
		std::vector<Metric> metrics = {
			{.name = "ball_pos_x"},
			{.name = "ball_pos_y"},
			{.name = "ball_pos_z"},
		};
	protected:
		virtual RLGSC::FList GetMetrics(RLGSC::GameState state) override;
	};
}

namespace PlayerLoggers {
	class PlayerSpeedLogger : public Logger {
	public:
		std::vector<Metric> metrics = {
			{.name = "player_speed"}
		};
	protected:
		virtual RLGSC::FList GetMetrics(RLGSC::GameState state) override;
	};

	class PlayerBallTouchLogger : public Logger {
	public:
		std::vector<Metric> metrics = {
			{.name = "ball_touch_ratio"}
		};
	protected:
		virtual RLGSC::FList GetMetrics(RLGSC::GameState state) override;
	};

	class PlayerInAirLogger : public Logger {
	public:
		std::vector<Metric> metrics = {
			{.name = "in_air_ratio"}
		};
	protected:
		virtual RLGSC::FList GetMetrics(RLGSC::GameState state) override;
	};
}