#pragma once

#include <RLGymSim_CPP/Utils/Gamestates/GameState.h>

#define VOID_TESTS_TAG "[Void_Tests]"

using namespace RLGSC;

class StateGenerator {
public:
	/// <summary>
	/// Creates a state where the first player is on the ground
	/// </summary>
	/// <param name="nBluePlayers">Number of blue players</param>
	/// <param name="nOrangePlayers">Number of orange players</param>
	/// <returns>A gamestate</returns>
	static GameState CreateGroundState(int nBluePlayers, int nOrangePlayers);

	/// <summary>
	/// Creates a state where the first player is in the air
	/// </summary>
	/// <param name="nBluePlayers">Number of blue players</param>
	/// <param name="nOrangePlayers">Number of orange players</param>
	/// <returns>A gamestate</returns>
	static GameState CreateAirState(int nBluePlayers, int nOrangePlayers);

	/// <summary>
	/// Creates a state where the first player is on the wall
	/// </summary>
	/// <param name="nBluePlayers">Number of blue players</param>
	/// <param name="nOrangePlayers">Number of orange players</param>
	/// <returns>A gamestate</returns>
	static GameState CreateOnWallState(int nBluePlayers, int nOrangePlayers);

	/// <summary>
	/// Creates a state where the first player is on the ceiling
	/// </summary>
	/// <param name="nBluePlayers">Number of blue players</param>
	/// <param name="nOrangePlayers">Number of orange players</param>
	/// <returns>A gamestate</returns>
	static GameState CreateOnCeilingState(int nBluePlayers, int nOrangePlayers);

	/// <summary>
	/// Creates a state wbere the first player is in the down left corner (based on rlbot's top view map)
	/// </summary>
	/// <param name="nBluePlayers">Number of blue players</param>
	/// <param name="nOrangePlayers">Number of orange players</param>
	/// <returns>A gamestate</returns>
	static GameState CreateOnCornerState(int nBluePlayers, int nOrangePlayers);
private:
	static RocketSim::Arena* CreateArena(int nBluePlayers, int nOrangePlayers);
};
