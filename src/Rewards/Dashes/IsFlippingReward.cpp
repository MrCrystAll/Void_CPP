#include <Rewards/Dashes/IsFlippingReward.h>

float IsFlippingReward::GetReward(const PlayerData& player, const GameState& state, const Action& prevAction)
{
	std::map<int, int> contacts;
	for (int i = 0; i < 5; i++) {
		contacts[player.carState.wheelsWithContact[i]]++; // increment the counter for the current value
	}

	if (contacts[1] == 2 and player.carState.hasJumped) {
		this->reward += {player.carState.isFlipping, "Is flipping"};
	}

	return this->ComputeReward();
}