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
    std::filesystem::path policyPath = exePath.parent_path() / "RLBotModel/Recovery/257108224/PPO_POLICY.lt";

	RLBotParams params = {
		.port = 42653,
		.obsBuilder = new DefaultOBS(),
		.actionParser = new RecoveryActionParser(),
		.policyPath = policyPath,
		.obsSize = 70,
		.policyLayerSizes = { 512, 512, 512 },
		.tickSkip = 2,
	};
	RLBotClient::Run(params);

    return 0;
}
