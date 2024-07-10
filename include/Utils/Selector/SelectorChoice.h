#pragma once

#include <RLGymPPO_CPP/Util/Report.h>
#include "Submodel.h"


class SelectorChoice {
public:
	SelectorChoice(std::vector<Submodel> submodels) : submodels(submodels) {};

	void UpdateCurrentSubmodels(std::vector<Submodel>& submodels);
	std::vector<Submodel> GetCurrentSubmodels();
	void Log(RLGPC::Report& report);

	std::vector<Submodel> GetSubmodels() { return this->submodels; };

private:
	std::vector<Submodel> currentSubmodels, submodels;
};