#pragma once

#include "LoggerUtils.h"
#include <map>
#include <vector>
#include <algorithm>
#include <numeric>

typedef std::pair<float, std::string> Log;

struct Stat
{

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

	float ComputeAvg() const;
	float ComputeMedian();
	float ComputeStd();
	float ComputeMin();
	float ComputeMax();

	StatResult ComputeAll();

	void Reset();

	Stat operator+(float value);
	Stat operator+=(float value);
};

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
	void Clear();

	void Step();
	void Reset();
	void ResetAll();
};