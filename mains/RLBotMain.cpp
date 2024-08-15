#include <Recovery/ActionParsers/RecoveryActionParser.h>
#include <Recovery/ObsBuilders/DashObsBuilder.h>

#include <RLGymSim_CPP/Utils/OBSBuilders/DefaultOBS.h>
#include <RLGymSim_CPP/Utils/ActionParsers/DiscreteAction.h>

#include <filesystem>
#include "RLBotClient.h"

using namespace RLGPC; // RLGymPPO
using namespace RLGSC; // RLGymSim

USE_RECOVERY_AP_NS;
USE_RECOVERY_OB_NS;


int main(int argc, char* argv[]) {
    std::filesystem::path exePath = std::filesystem::path(argv[0]);
    std::filesystem::path policyPath = exePath.parent_path() / "RLBotModel/PPO_POLICY.lt";

	RLBotParams params = {
		.port = 42653,
		.obsBuilder = new DefaultOBS(),
		.actionParser = new DiscreteAction(),
		.policyPath = policyPath,
		.obsSize = 165,
		.policyLayerSizes = { 256, 256, 256 },
		.tickSkip = 2
	};
	RLBotClient::Run(params);

    return 0;
}
