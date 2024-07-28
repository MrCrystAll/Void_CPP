#include <Selector/SelectorChoice.h>
#include "Utils/LoggerUtils.h"

USE_SELECTOR_NS;

void SelectorChoice::UpdateCurrentSubmodels(std::vector<Submodel>& actions)
{
	this->currentSubmodels = actions;
}

std::vector<Submodel> SelectorChoice::GetCurrentSubmodels()
{
	return this->currentSubmodels;
}

void SelectorChoice::Log(RLGPC::Report& report)
{
	for (Submodel s : this->currentSubmodels) {
		report.Accum(SELECTOR_HEADER + s.GetName(), 1);
	}
}
