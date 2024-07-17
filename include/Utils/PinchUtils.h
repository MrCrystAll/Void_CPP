/*****************************************************************//**
 * \file   PinchUtils.h
 * \brief  All the utilities for the pinch
 * 
 * \author Mathieu Suchet
 * \date   July 2024
 *********************************************************************/

#pragma once

#include <Utils/VoidUtils.h>

/**
 * @brief Namespace name
 */
#define PINCH_NS Pinch

/**
 * @brief Start flag
 */
#define START_REWARDS_PINCH_NS /**\
 * @brief Pinch namespace \
  */ START_REWARDS_SUB_NS(PINCH_NS)
#define START_STATES_PINCH_NS /**\
 * @brief Pinch namespace \
  */ START_STATES_SUB_NS(PINCH_NS)

/**
 * @brief End flag
 */
#define END_PINCH_NS };

/**
 * @brief Use flag
 */
#define USE_REWARDS_PINCH_NS USE_REWARDS_SUB_NS(PINCH_NS);
#define USE_STATES_PINCH_NS USE_STATES_SUB_NS(PINCH_NS);
#define USE_ACTION_PARSER_PINCH_NS USE_ACTION_PARSER_SUB_NS(PINCH_NS);
#define USE_OBS_BUILDERS_PINCH_NS USE_OBS_BUILDER_SUB_NS(PINCH_NS);
