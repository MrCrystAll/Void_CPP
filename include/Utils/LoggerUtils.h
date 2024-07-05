#include <string>
#pragma once
constexpr auto REWARD_HEADER = "Rewards/"; 
constexpr auto METRICS_HEADER = "Metrics/";

#define VOID_LOG(content) std::cout << content << std::endl
#define VOID_WARN(content) std::cout << "[WARN] - " << content << std::endl
#define VOID_ERR(content) std::cerr << "[ERROR] - " << content << std::endl

#define DECOMPOSED_VECTOR(v) v.x, v.y, v.z