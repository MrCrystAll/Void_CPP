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

bool DribbleRCF::PlayerFilter(const PlayerData player, const GameState state)
{
    PhysObj ball = state.ball;
    
    Vec tolerance = Vec(50, 75, 250);
    Vec ballPlayerDiff = state.ball.pos - player.carState.pos;
    
    return std::abs(ballPlayerDiff.x) < tolerance.x 
        and std::abs(ballPlayerDiff.y) < tolerance.y 
        and ballPlayerDiff.z < tolerance.z and ballPlayerDiff.z > 100 and player.carState.pos.z < 150 and player.carState.isOnGround;
}

bool ShootingRCF::PlayerFilter(const PlayerData player, const GameState state)
{
    int lastMatchShot = lastMatchShots.contains(player.carId) ? lastMatchShots[player.carId] : 0;
    bool result = player.matchShots > lastMatchShot;
    lastMatchShots[player.carId] = player.matchShots;
    return result;
}

bool SavingRCF::PlayerFilter(const PlayerData player, const GameState state)
{
    int lastMatchSave = lastMatchSaves.contains(player.carId) ? lastMatchSaves[player.carId] : 0;
    bool result = player.matchSaves > lastMatchSave;
    lastMatchSaves[player.carId] = player.matchSaves;
    return result;
}

bool FlipResetRCF::PlayerFilter(const PlayerData player, const GameState state)
{
    return player.carState.HasFlipReset();
}
