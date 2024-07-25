/*****************************************************************//**
 * \file   SelectorChoice.h
 * \brief  Class used to log and get the currently used submodels
 * 
 * \author msuch
 * \date   July 2024
 *********************************************************************/

#pragma once

#include <RLGymPPO_CPP/Util/Report.h>
#include "Submodel.h"
#include <Utils/Selector/SelectorUtils.h>

START_SELECTOR_NS

/// <summary>
/// Class used to log and get the currently used submodels
/// </summary>
class SelectorChoice {
public:
	SelectorChoice(std::vector<Submodel> submodels) : submodels(submodels) {};

	/// <summary>
	/// Updates the currently used submodels
	/// </summary>
	/// <param name="submodels">Used submodels</param>
	void UpdateCurrentSubmodels(std::vector<Submodel>& submodels);

	/// <summary>
	/// Get all the currently used submodels
	/// </summary>
	/// <returns>The currently used submodels</returns>
	std::vector<Submodel> GetCurrentSubmodels();

	/// <summary>
	/// Logs everything to the report
	/// </summary>
	/// <param name="report">The report to log to</param>
	void Log(RLGPC::Report& report);

	/// <summary>
	/// Get all the possible submodels that the selector can sue
	/// </summary>
	/// <returns>The submodels the selector can use</returns>
	std::vector<Submodel> GetSubmodels() { return this->submodels; };

private:
	std::vector<Submodel> currentSubmodels, submodels;
};

END_SELECTOR_NS
