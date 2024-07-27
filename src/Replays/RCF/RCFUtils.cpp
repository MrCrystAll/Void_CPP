#include <Replays/RCF/RCFUtils.h>
#include <RLGymSim_CPP/Utils/TerminalConditions/GoalScoreCondition.h>

START_RCF_NS

#define X_AT_ZERO 8064

bool isGoalScored(const GameState state)
{
	return GoalScoreCondition().IsTerminal(state);
}

bool IsOnWall(const PlayerData player, int wallHeightTolerance, int wallWidthTolerance)
{
	bool isOnFlatWall = player.carState.isOnGround;
    isOnFlatWall = isOnFlatWall and RLGSC::CommonValues::SIDE_WALL_X - wallWidthTolerance
        < std::abs(player.carState.pos.x) and std::abs(player.carState.pos.x)
        < RLGSC::CommonValues::SIDE_WALL_X + wallWidthTolerance
        // Back wall comparison
        or RLGSC::CommonValues::BACK_WALL_Y - wallWidthTolerance
        < std::abs(player.carState.pos.y) and std::abs(player.carState.pos.y)
        < RLGSC::CommonValues::BACK_WALL_Y + wallWidthTolerance;

    isOnFlatWall = isOnFlatWall and wallHeightTolerance
        < player.carState.pos.z and player.carState.pos.z
        < RLGSC::CommonValues::CEILING_Z - wallHeightTolerance;

    if (isOnFlatWall) return true;

    bool isOnCorner = false;

    for (int a : {-1, 1}) {
        if (isOnCorner) break;

        for (int b : {-1, 1}) {
            if (player.carState.pos.y - wallWidthTolerance
                < a * player.carState.pos.x + (X_AT_ZERO * b) and a * player.carState.pos.x + (X_AT_ZERO * b)
                < player.carState.pos.y + wallWidthTolerance) {
                isOnCorner = true;
                break;
            }
        }
    }

    return isOnCorner;
}

bool IsOnCeiling(const PlayerData player, int wallHeightTolerance, int wallWidthTolerance)
{
    return player.carState.isOnGround
        and std::abs(player.carState.pos.x) < RLGSC::CommonValues::SIDE_WALL_X - wallWidthTolerance
        and std::abs(player.carState.pos.y) < RLGSC::CommonValues::BACK_WALL_Y - wallWidthTolerance
        and player.carState.pos.z > RLGSC::CommonValues::CEILING_Z - wallHeightTolerance;
}

END_RCF_NS



