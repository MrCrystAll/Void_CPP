#include <Replays/ReplayUtils.h>
#include <__msvc_chrono.hpp>

START_REPLAY_NS;

std::string GetTimePath()
{
	auto end = std::chrono::system_clock::now();
	std::time_t time = std::chrono::system_clock::to_time_t(end);
	std::tm* localTime = std::localtime(&time);
	return std::to_string(localTime->tm_mday) + "_" + std::to_string(localTime->tm_hour) + "_" + std::to_string(localTime->tm_min) + "_" + std::to_string(localTime->tm_sec);
}

END_REPLAY_NS