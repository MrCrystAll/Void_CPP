/*****************************************************************//**
 * \file   DashesUtils.h
 * \brief  All utils for dashes
 * 
 * \author Mathieu Suchet
 * \date   July 2024
 *********************************************************************/
#pragma once

#include <Utils/VoidUtils.h>
#include <Utils/LoggingUtils.h>

 /**
  * @brief Namespace name
  */
#define DASHES_NS Dashes

  /**
   * @brief Start flag
   */
#define START_REWARDS_DASHES_NS /**\
 * @brief Double tap reward namespace\
 */START_REWARDS_SUB_NS(DASHES_NS)

#define START_STATES_DASHES_NS /**\
 * @brief Double tap state namespace\
 */START_STATES_SUB_NS(DASHES_NS)

   /** @brief End flag */
#define END_DASHES_NS };

/**
 * @brief Use flag
 */
#define USE_REWARDS_DASHES_NS USE_REWARDS_SUB_NS(DASHES_NS);
#define USE_STATES_DASHES_NS USE_STATES_SUB_NS(DASHES_NS);
#define USE_ACTION_PARSER_DASHES_NS USE_ACTION_PARSER_SUB_NS(DASHES_NS);
#define USE_OBS_BUILDERS_DASHES_NS USE_OBS_BUILDER_SUB_NS(DASHES_NS);

USE_LOGGING_NS;
