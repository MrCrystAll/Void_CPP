#pragma once

#include "Utils/LoggerUtils.h"
#include <map>
#include <algorithm>
#include <cmath>
#include "Utils/LoggingUtils.h"

LoggedFloat LoggedFloat::operator+(Log other)
{
	this->value += other.first;
	if (this->logs.contains(other.second)) 
	{
		this->logs[other.second] += other.first;
	}
	else
	{
		this->logs[other.second] = { .value = other.first, .count = this->nbSteps };
		this->metrics.push_back(other.second);
	}

	
	this->logs["_total"] += other.first;
	//VOID_LOG("Logging " << other.second << ": " << other.first << " (Total: " << this->logs["_total"].value << ")");
	return *this;
}

LoggedFloat LoggedFloat::operator+=(Log other)
{
	return *this + other;
}

float Stat::ComputeAvg() const { return count > 0 ? value / count : 0; }

float Stat::ComputeMedian() {
	std::sort(allValues.begin(), allValues.end(), std::less_equal<float>());
	int n = allValues.size();

	if (n % 2 != 0)
		return allValues[n / 2];

	// If the number of elements is even, return the average 
	// of the two middle elements 
	return (allValues[(n - 1) / 2] + allValues[n / 2]) / 2.0;
}

float Stat::ComputeStd() {
	double sum = std::accumulate(allValues.begin(), allValues.end(), 0.0);
	double mean = sum / allValues.size();

	double sq_sum = std::inner_product(allValues.begin(), allValues.end(), allValues.begin(), 0.0);
	double stdev = std::sqrt(sq_sum / allValues.size() - mean * mean);
	return (float)stdev;
}

float Stat::ComputeMin() {
	return *std::min_element(allValues.begin(), allValues.end());
}

float Stat::ComputeMax() {
	return *std::max_element(allValues.begin(), allValues.end());
}

Stat::StatResult Stat::ComputeAll() {
	return { .mean = ComputeAvg(), .median = ComputeMedian(), .std = ComputeStd(), .min = ComputeMin(), .max = ComputeMax() };
}

void Stat::Reset() { value = 0; count = 0; this->allValues.clear(); }

Stat Stat::operator+(float value) { this->value += value; allValues.push_back(value); return *this; }
Stat Stat::operator+=(float value) { return *this + value; };


void LoggedFloat::InitMetrics() {
	for (std::string s : metrics) {
		logs[s] = {};
	}
	logs["_total"] = {};
}

void LoggedFloat::Clear() { this->logs.clear(); }

void LoggedFloat::Step() {
	for (auto& [key, val] : this->logs) {
		if (val.allValues.size() < val.count) val.allValues.push_back(0.0f);
		val.count++;
		nbSteps++;
	}
}

void LoggedFloat::Reset() { this->value = 0; }
void LoggedFloat::ResetAll() {
	this->logs.clear();
	this->InitMetrics();
	this->Reset();
}