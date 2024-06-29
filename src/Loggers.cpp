#include "Loggers.h"
#include <LoggerUtils.h>

void Logger::Log(RLGPC::Report& report, RLGSC::GameState state)
{
	RLGSC::FList data = GetMetrics(state);
	int size = data.size();
	if (data.size() != metrics.size()) {
		VOID_ERR("Expected " << metrics.size() << " metrics, got " << data.size());
		size = data.size() > metrics.size() ? metrics.size() : data.size();
	}

	for (int i = 0; i < size; i++) {
		if (metrics[i].isAvg) {
			report.AccumAvg(METRICS_HEADER + metrics[i].name, data[i]);
		}
		else {
			report.Accum(METRICS_HEADER + metrics[i].name, data[i]);
		}
	}
}

RLGSC::FList BallLoggers::BallSpeedLogger::GetMetrics(RLGSC::GameState state)
{
	return { state.ball.vel.Length() };
}

RLGSC::FList BallLoggers::BallVelocityLogger::GetMetrics(RLGSC::GameState state)
{
	return { DECOMPOSED_VECTOR(state.ball.vel) };
}


RLGSC::FList BallLoggers::BallPositionLogger::GetMetrics(RLGSC::GameState state)
{
	return { DECOMPOSED_VECTOR(state.ball.pos) };
}

RLGSC::FList PlayerLoggers::PlayerSpeedLogger::GetMetrics(RLGSC::GameState state)
{
	float sum = 0;
	for (const RLGSC::PlayerData& p : state.players) {
		sum += p.carState.vel.Length();
	}

	return { sum / state.players.size() };
}

RLGSC::FList PlayerLoggers::PlayerBallTouchLogger::GetMetrics(RLGSC::GameState state)
{
	float sum = 0;
	for (const RLGSC::PlayerData& p : state.players) {
		sum += p.ballTouchedStep;
	}

	return { sum / state.players.size() };
}

RLGSC::FList PlayerLoggers::PlayerInAirLogger::GetMetrics(RLGSC::GameState state)
{

	float sum = 0;
	for (const RLGSC::PlayerData& p : state.players) {
		sum += !p.carState.isOnGround;
	}

	return { sum / state.players.size() };
}

RLGSC::FList BallLoggers::BallHeightLogger::GetMetrics(RLGSC::GameState state)
{
	return {state.ball.pos.z};
}

RLGSC::FList PlayerLoggers::PlayerHeightLogger::GetMetrics(RLGSC::GameState state)
{
	float sum = 0;
	for (const RLGSC::PlayerData& p : state.players) {
		sum += p.carState.pos.z;
	}

	return { sum / state.players.size() };
}

RLGSC::FList PlayerLoggers::DemoLogger::GetMetrics(RLGSC::GameState state)
{
	float nDemos = 0;
	for (const RLGSC::PlayerData& p : state.players) {
		if (p.carState.isDemoed) nDemos++;
	}

	return { nDemos };
}
