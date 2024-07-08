#pragma once
#include <string>

#include <RLGymSim_CPP/Utils/ActionParsers/ActionParser.h>
#include <RLGymSim_CPP/Utils/ActionParsers/DiscreteAction.h>

#include <RLGymSim_CPP/Utils/OBSBuilders/OBSBuilder.h>
#include <RLGymSim_CPP/Utils/OBSBuilders/DefaultOBS.h>

#include <torch/csrc/jit/api/module.h>

class Submodel {
public:
	Submodel(std::string path, RLGSC::OBSBuilder* obsBuilder = new RLGSC::DefaultOBS(), RLGSC::ActionParser* parser = new RLGSC::DiscreteAction()) : obsBuilder(obsBuilder), parser(parser) {
		Load(path);
	};

	RLGSC::ActionSet Act(RLGSC::FList obs, bool deterministic = false);

private:
	void Load(std::string path);

	RLGSC::ActionParser* parser;
	RLGSC::OBSBuilder* obsBuilder;
	torch::jit::Module actor;
};
