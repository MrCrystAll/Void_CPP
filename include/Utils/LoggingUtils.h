/*****************************************************************//**
 * \file   LoggingUtils.h
 * \brief  All the utilities for the reward logging
 * 
 * \author Mathieu Suchet
 * \date   July 2024
 *********************************************************************/

#pragma once

#include "LoggerUtils.h"
#include <map>
#include <vector>
#include <algorithm>
#include <numeric>

typedef std::pair<float, std::string> Log;

/**
 * @brief Stat
 * Holds all the statistics calculations
 */
struct Stat
{
	/**
	 * @brief StatResult
	 * Holds the statistics results from the calculations
	 */
	struct StatResult {
		float mean;
		float median;
		float std;
		float min;
		float max;
	};

	float value = 0;
	unsigned int count = 0;

	std::vector<float> allValues = std::vector<float>(count, 0.0f);

	/**
	 * @brief Computes the average
	 * 
	 * \return The average of the stored values
	 */
	float ComputeAvg() const;

	/**
	 * @brief Computes the median.
	 * 
	 * \return The median of the stored values
	 */
	float ComputeMedian();

	/**
	 * @brief Computes the standard deviation
	 * 
	 * \return The standard deviation of the stored values
	 */
	float ComputeStd();

	/**
	 * @brief Computes the min
	 * 
	 * \return The minimum of the stored values
	 */
	float ComputeMin();

	/**
	 * @brief Computes the max
	 * 
	 * \return The maximum of the stored values
	 */
	float ComputeMax();

	/**
	 * @brief Compute everything
	 * 
	 * \return All the computations
	 */
	StatResult ComputeAll();

	/**
	 * @brief Resets the stat
	 * 
	 */
	void Reset();

	Stat operator+(float value);
	Stat operator+=(float value);
};

/**
 * @brief A float you can keep the changes of
 */
struct LoggedFloat
{
	float value = 0;
	unsigned int nbSteps = 0;
	std::map<std::string, Stat> logs = {};
	std::vector<std::string> metrics = {};

	LoggedFloat operator+(Log other);
	LoggedFloat operator+=(Log other);

	LoggedFloat(std::vector<std::string> metrics = {}) : metrics(metrics) { InitMetrics(); };

	void InitMetrics();

	/**
	 * @brief Clears the logs
	 * 
	 */
	void Clear();

	/**
	 * @brief Steps the float to keep track of the number of updates.
	 * 
	 */
	void Step();

	/**
	 * @brief Resets the value
	 * 
	 */
	void Reset();

	/**
	 * @brief Resets the value, clears the logs and reinitialize the metrics.
	 * 
	 */
	void ResetAll();
};