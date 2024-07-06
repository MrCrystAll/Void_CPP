/*****************************************************************//**
 * \file   LoggerUtils.h
 * \brief  All the utils for the logger (console log, metrics and reward headers)
 * 
 * \author Mathieu Suchet
 * \date   July 2024
 *********************************************************************/

#include <string>
#pragma once

/**
 * @brief The header to organize the rewards
 */
constexpr auto REWARD_HEADER = "Rewards/"; 

/**
 * @brief The header to organize the metrics
 */
constexpr auto METRICS_HEADER = "Metrics/";

/**
 * @brief Logs the content on the out channel (Used for normal logs)
 */
#define VOID_LOG(content) std::cout << content << std::endl

/**
 * @brief Logs the content on the out channel with a warning flag
 */
#define VOID_WARN(content) std::cout << "[WARN] - " << content << std::endl

/**
 * @brief Logs the content on the err channel with an error flag
 */
#define VOID_ERR(content) std::cerr << "[ERROR] - " << content << std::endl

/**
 * @brief Writes down all the components of a given vector
 * 
 * @param v The vector to decompose
 */
#define DECOMPOSED_VECTOR(v) v.x, v.y, v.z