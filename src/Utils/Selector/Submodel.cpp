#include "Utils/Selector/Submodel.h"
#include <torch/csrc/jit/serialization/import.h>
#include <torch/torch.h>

RLGSC::ActionSet Submodel::Act(RLGSC::FList obs, bool deterministic)
{
	//Transform into obs
	torch::Tensor tensorObs = torch::tensor(obs);

	std::vector<torch::jit::IValue> tensors = { tensorObs };

	//Inference
	torch::Tensor resultsActions = this->actor.forward(tensors).toTensor();

	return RLGSC::ActionSet();
}

void Submodel::Load(std::string path)
{
	this->actor = torch::jit::load(path);
}
