#include <Utils/VoidUtils.h>
#include <Logging/LoggableReward.h>

//Base namespace utils (Auto-generated comment)
#define DOUBLETAP_NS DoubleTap
#define START_DOUBLETAP_NS START_VOID_SUB_NS(DOUBLETAP_NS)
#define END_DOUBLETAP_NS END_NS

//Sub namespaces start macros (Auto-generated comment)
#define START_DOUBLETAP_REWARDS_NS START_VOID_SUB_NS(DOUBLETAP_NS::Rewards)
#define START_DOUBLETAP_STATES_NS START_VOID_SUB_NS(DOUBLETAP_NS::States)
#define START_DOUBLETAP_AP_NS START_VOID_SUB_NS(DOUBLETAP_NS::AP)
#define START_DOUBLETAP_OB_NS START_VOID_SUB_NS(DOUBLETAP_NS::OB)

//Sub namespaces use macros (Auto-generated comment)
#define USE_DOUBLETAP_REWARDS_NS USE_VOID_SUB_NS(DOUBLETAP_NS::Rewards)
#define USE_DOUBLETAP_STATES_NS USE_VOID_SUB_NS(DOUBLETAP_NS::States)
#define USE_DOUBLETAP_AP_NS USE_VOID_SUB_NS(DOUBLETAP_NS::AP)
#define USE_DOUBLETAP_OB_NS USE_VOID_SUB_NS(DOUBLETAP_NS::OB)

START_DOUBLETAP_NS

#include "RLGymSim_CPP/Utils/Gamestates/PhysObj.h"

/**
 * @brief Checks if the physics object is within the backboard zone
 *
 * @param physObj The object to check the position of
 * @param distFromBackboard The maximum distance from the back wall
 * @param minHeight The minimum height from the ground
 */
bool IsOutsideBackboardZone(const RLGSC::PhysObj physObj, int distFromBackboard, int minHeight);

END_DOUBLETAP_NS

USE_LOGGING_NS;