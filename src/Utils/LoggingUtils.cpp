#pragma once

#include "Utils/LoggerUtils.h"
#include <map>
#include <algorithm>
#include <cmath>
#include <iostream>
#include "Utils/LoggingUtils.h"

USE_LOGGING_NS;

LoggedFloat LoggedFloat::operator+(Log other)
{
	this->value += other.first;
	if (not this->logs.contains(other.second))
	{
		this->logs[other.second] = { .count = this->nbSteps };
		this->metrics.push_back(other.second);

	}

	this->logs[other.second] += other.first;

	
	this->logs["_total"] += other.first;
	return *this;
}

LoggedFloat LoggedFloat::operator+=(Log other)
{
	return *this + other;
}

LoggedFloat LoggedFloat::operator-(Log other)
{
	this->value -= other.first;
	if (not this->logs.contains(other.second))
	{
		this->logs[other.second] = { .count = this->nbSteps };
		this->metrics.push_back(other.second);

	}

	this->logs[other.second] -= other.first;


	this->logs["_total"] -= other.first;
	return *this;
}

LoggedFloat LoggedFloat::operator-=(Log other)
{
	return *this - other;
}

LoggedFloat LoggedFloat::operator*(Log other)
{
	float difference = (other.first - 1) * this->value;
	this->value += difference;
	if (not this->logs.contains(other.second))
	{
		this->logs[other.second] = { .count = this->nbSteps };
		this->metrics.push_back(other.second);

	}

	this->logs[other.second] += difference;
	
	this->logs["_total"] += difference;
	return *this;
}

LoggedFloat LoggedFloat::operator*=(Log other)
{
	return *this * other;
}

LoggedFloat LoggedFloat::operator/(Log other)
{
	//This is negative (unless you divide by a negative number, then it's positive)
	float difference = (this->value * (1 - other.first)) / other.first;

	this->value += difference;
	if (not this->logs.contains(other.second))
	{
		this->logs[other.second] = { .count = this->nbSteps };
		this->metrics.push_back(other.second);
		
	}
	
	this->logs[other.second] += difference;


	this->logs["_total"] += other.first;
	return *this;
}

LoggedFloat LoggedFloat::operator/=(Log other)
{
	return *this / other;
}

float Stat::ComputeAvg() const { 
	double sum = std::accumulate(allValues.begin(), allValues.end(), 0.0);
	return sum / count;
}

float Stat::ComputeMedian() {
	std::sort(allValues.begin(), allValues.end(), std::less_equal<float>());
	size_t n = allValues.size();

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

Stat Stat::operator+(float value) { this->value += value; allValues.push_back(value); return *this; }
Stat Stat::operator+=(float value) { return *this + value; }
Stat Stat::operator-(float value) { this->value -= value; allValues.push_back(-value); return *this; }
Stat Stat::operator-=(float value) { return *this - value; }

Stat::~Stat()
{
	this->allValues.clear();

	//I mean, that's dumb
	this->count = 0;
}

void LoggedFloat::InitMetrics() {
	for (std::string s : metrics) {
		logs[s] = {};
	}
	logs["_total"] = {};
}

void LoggedFloat::Clear() { this->logs.clear(); }

void LoggedFloat::Step() {
	nbSteps++;
	for (auto& [key, val] : this->logs) {
		if (val.allValues.size() < val.count) val += 0;
		/*std::cout << key << ": ";
		for (float f : val.allValues) {
			std::cout << f << ", ";
		}
		std::cout << std::endl;*/
		val.count++;
	}
}

void LoggedFloat::Reset() { 
	this->value = 0;
}
void LoggedFloat::ResetAll() {
	this->logs.clear();
	this->InitMetrics();
	this->Reset();
}