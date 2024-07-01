#include "Utils/DoubleTapUtils.h"
#include "RLGymSim_CPP/Utils/Gamestates/PhysObj.h"
#include "RLGymSim_CPP/Utils/CommonValues.h"

bool IsOutsideBackboardZone(const RLGSC::PhysObj physObj, int distFromBackboard, int minHeight)
{
    return not (std::abs(physObj.pos.y) > RLGSC::CommonValues::BACK_WALL_Y - distFromBackboard and std::abs(physObj.pos.y) < RLGSC::CommonValues::BACK_WALL_Y - 200 and physObj.pos.z > minHeight);
}