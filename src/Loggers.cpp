#include "Loggers.h"
#include <LoggerUtils.h>

void Logger::Log(RLGPC::Report& report, RLGSC::GameState state)
{
	RLGSC::FList data = GetMetrics(state);
	int size = data.size();
	if (data.size() != metrics.size()) {
		VOID_WARN("Expected " << metrics.size() << " metrics, got " << data.size());
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
	VOID_LOG(metrics[0].name);
	return { state.ball.vel.Length() };
}

RLGSC::FList BallLoggers::BallVelocityLogger::GetMetrics(RLGSC::GameState state)
{
	VOID_LOG(metrics[0].name);
	return { DECOMPOSED_VECTOR(state.ball.vel) };
}


RLGSC::FList BallLoggers::BallPositionLogger::GetMetrics(RLGSC::GameState state)
{
	VOID_LOG(metrics[0].name);
	return { DECOMPOSED_VECTOR(state.ball.pos) };
}

RLGSC::FList PlayerLoggers::PlayerSpeedLogger::GetMetrics(RLGSC::GameState state)
{
	VOID_LOG(metrics[0].name);
	float sum = 0;
	for (const RLGSC::PlayerData& p : state.players) {
		sum += p.carState.vel.Length();
	}

	return { sum / state.players.size() };
}

RLGSC::FList PlayerLoggers::PlayerBallTouchLogger::GetMetrics(RLGSC::GameState state)
{
	VOID_LOG(metrics[0].name);
	float sum = 0;
	for (const RLGSC::PlayerData& p : state.players) {
		sum += p.ballTouchedStep;
	}

	return { sum / state.players.size() };
}

RLGSC::FList PlayerLoggers::PlayerInAirLogger::GetMetrics(RLGSC::GameState state)
{
	VOID_LOG(metrics[0].name);

	float sum = 0;
	for (const RLGSC::PlayerData& p : state.players) {
		sum += !p.carState.isOnGround;
	}

	return { sum / state.players.size() };
}