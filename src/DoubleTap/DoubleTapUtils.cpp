#include <DoubleTap/DoubleTapUtils.h>

START_DOUBLETAP_NS

bool IsOutsideBackboardZone(const RLGSC::PhysObj physObj, int distFromBackboard, int minHeight)
{
    return not (std::abs(physObj.pos.y) > RLGSC::CommonValues::BACK_WALL_Y - distFromBackboard and std::abs(physObj.pos.y) < RLGSC::CommonValues::BACK_WALL_Y - 200 and physObj.pos.z > minHeight);
}

END_DOUBLETAP_NS