#pragma once

#define DT_NS DoubleTap
#define START_DT_NS namespace DT_NS {
#define END_DT_NS };
#define USE_DT_NS using namespace DT_NS;

#include "RLGymSim_CPP/Utils/Gamestates/PhysObj.h"

bool IsOutsideBackboardZone(const RLGSC::PhysObj physObj, int distFromBackboard, int minHeight);