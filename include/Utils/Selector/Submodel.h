#pragma once
#include <string>

#include <RLGymSim_CPP/Utils/ActionParsers/ActionParser.h>
#include <RLGymSim_CPP/Utils/ActionParsers/DiscreteAction.h>

#include <RLGymSim_CPP/Utils/OBSBuilders/OBSBuilder.h>
#include <RLGymSim_CPP/Utils/OBSBuilders/DefaultOBS.h>

#include <RLGymPPO_CPP/Util/PolicyInferUnit.h>

struct SubmodelArgs {
	std::string path;
	std::string name;
	int obsSize;
	float weight;
	RLGSC::OBSBuilder* obsBuilder = new RLGSC::DefaultOBS();
	RLGSC::ActionParser* parser = new RLGSC::DiscreteAction();
	RLGPC::IList policyLayerSizes = { 256, 256, 256 };
	bool deterministic = true;
};

class Submodel {
public:
	Submodel(SubmodelArgs args) : obsBuilder(args.obsBuilder), parser(args.parser), agent(args.obsBuilder, args.parser, args.path, args.obsSize, args.policyLayerSizes, args.deterministic), name(args.name), weight(args.weight), deterministic(args.deterministic) {

	};

	RLGSC::OBSBuilder* GetObsBuilder() const { return this->obsBuilder; };
	RLGSC::ActionParser* GetActionParser() const { return this->parser; };

	std::string GetName() const { return this->name; };
	float GetWeight() const { return this->weight; };

	RLGSC::ActionSet Act(const RLGSC::GameState& state, const RLGSC::ActionSet& prevActions, bool deterministic);

private:
	RLGSC::ActionParser* parser;
	RLGSC::OBSBuilder* obsBuilder;
	RLGPC::PolicyInferUnit agent;
	std::string name;
	float weight;
	bool deterministic;
};
