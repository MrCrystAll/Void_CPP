/*****************************************************************//**
 * \file   Submodel.h
 * \brief  The class handling the submodels
 * 
 * \author msuch
 * \date   July 2024
 *********************************************************************/

#pragma once
#include <string>

#include <RLGymSim_CPP/Utils/ActionParsers/ActionParser.h>
#include <RLGymSim_CPP/Utils/ActionParsers/DiscreteAction.h>

#include <RLGymSim_CPP/Utils/OBSBuilders/OBSBuilder.h>
#include <RLGymSim_CPP/Utils/OBSBuilders/DefaultOBS.h>

#include <RLGymPPO_CPP/Util/PolicyInferUnit.h>
#include <Utils/Selector/SelectorUtils.h>

START_SELECTOR_NS

/// <summary>
/// The arguments used for the submodel
/// </summary>
struct SubmodelArgs {
	/// <summary>
	/// Path to the submodel
	/// </summary>
	std::string path;

	/// <summary>
	/// Name of the submodel
	/// </summary>
	std::string name;

	/// <summary>
	/// Size of the obs
	/// </summary>
	int obsSize;

	/// <summary>
	/// Weight of the obs (how rewarding it is for the selector to pick it)
	/// </summary>
	float weight;

	/// <summary>
	/// Observation builder used by the submodel
	/// </summary>
	RLGSC::OBSBuilder* obsBuilder = new RLGSC::DefaultOBS();

	/// <summary>
	/// Action parser used by the submodel
	/// </summary>
	RLGSC::ActionParser* parser = new RLGSC::DiscreteAction();

	/// <summary>
	/// Submodel size
	/// </summary>
	RLGPC::IList policyLayerSizes = { 256, 256, 256 };

	/// <summary>
	/// Whether the submodel acts deterministically
	/// </summary>
	bool deterministic = true;
};

/// <summary>
/// The class handling the submodels
/// </summary>
class Submodel {
public:
	Submodel(SubmodelArgs args) : obsBuilder(args.obsBuilder), parser(args.parser), agent(args.obsBuilder, args.parser, args.path, args.obsSize, args.policyLayerSizes, args.deterministic), name(args.name), weight(args.weight), deterministic(args.deterministic) {

	};

	RLGSC::OBSBuilder* GetObsBuilder() const { return this->obsBuilder; };
	RLGSC::ActionParser* GetActionParser() const { return this->parser; };

	std::string GetName() const { return this->name; };
	float GetWeight() const { return this->weight; };

	/// <summary>
	/// Lets the submodel act on a state
	/// </summary>
	/// <param name="state">The state to act on</param>
	/// <param name="prevActions">Previous actions</param>
	/// <param name="deterministic">Deterministic or not</param>
	/// <returns>The action of the submodel</returns>
	RLGSC::ActionSet Act(const RLGSC::GameState& state, const RLGSC::ActionSet& prevActions, bool deterministic);

private:
	RLGSC::ActionParser* parser;
	RLGSC::OBSBuilder* obsBuilder;
	RLGPC::PolicyInferUnit agent;
	std::string name;
	float weight;
	bool deterministic;
};

END_SELECTOR_NS