/*****************************************************************//**
 * \file   DoubleTapUtils.h
 * \brief  All utilities for double taps
 * 
 * \author Mathieu Suchet
 * \date   July 2024
 *********************************************************************/

#pragma once

#include <Utils/VoidUtils.h>

/**
 * @brief Namespace name
 */
#define DT_NS DoubleTap

/**
 * @brief Start flag
 */
#define START_REWARDS_DT_NS /**\
 * @brief Double tap reward namespace\
 */START_REWARDS_SUB_NS(DT_NS)
#define START_STATES_DT_NS /**\
 * @brief Double tap state namespace\
 */START_STATES_SUB_NS(DT_NS)

/** @brief End flag */
#define END_DT_NS };

/**
 * @brief Use flag
 */
#define USE_REWARDS_DT_NS USE_REWARDS_SUB_NS(DT_NS);
#define USE_STATES_DT_NS USE_STATES_SUB_NS(DT_NS);
#define USE_ACTION_PARSER_DT_NS USE_ACTION_PARSER_SUB_NS(DT_NS);
#define USE_OBS_BUILDERS_DT_NS USE_OBS_BUILDER_SUB_NS(DT_NS);

#include "RLGymSim_CPP/Utils/Gamestates/PhysObj.h"

/**
 * @brief Checks if the physics object is within the backboard zone
 * 
 * @param physObj The object to check the position of
 * @param distFromBackboard The maximum distance from the back wall
 * @param minHeight The minimum height from the ground
 */
bool IsOutsideBackboardZone(const RLGSC::PhysObj physObj, int distFromBackboard, int minHeight);




