#include <Replays/RCF/CommonRCFs.h>

USE_RCF_NS;

bool OnGroundRCF::PlayerFilter(const PlayerData player, const GameState state)
{
    return player.carState.isOnGround;
}

bool OnWallRCF::PlayerFilter(const PlayerData player, const GameState state)
{
    return IsOnWall(player, this->wallHeightTolerance, this->wallWidthTolerance);
}

bool OnCeilingRCF::PlayerFilter(const PlayerData player, const GameState state)
{
    return IsOnCeiling(player, this->wallHeightTolerance, this->wallWidthTolerance);
}

bool SupersonicRCF::PlayerFilter(const PlayerData player, const GameState state)
{
    return player.carState.vel.Length() >= RLGSC::CommonValues::SUPERSONIC_THRESHOLD;
}
