/*****************************************************************//**
 * \file   DoubleTapUtils.h
 * \brief  All utilities for double taps
 * 
 * \author Mathieu Suchet
 * \date   July 2024
 *********************************************************************/

#pragma once

/**
 * @brief Namespace name
 */
#define DT_NS DoubleTap

/**
 * @brief Start flag
 */
#define START_DT_NS namespace DT_NS {

/** @brief End flag */
#define END_DT_NS };

/**
 * @brief Use flag
 */
#define USE_DT_NS using namespace DT_NS;

#include "RLGymSim_CPP/Utils/Gamestates/PhysObj.h"

/**
 * @brief Checks if the physics object is within the backboard zone
 * 
 * @param physObj The object to check the position of
 * @param distFromBackboard The maximum distance from the back wall
 * @param minHeight The minimum height from the ground
 */
bool IsOutsideBackboardZone(const RLGSC::PhysObj physObj, int distFromBackboard, int minHeight);




