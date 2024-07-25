/*****************************************************************//**
 * \file   VoidUtils.h
 * \brief  All namespaces and utils used for Void
 * 
 * \author Mathieu Suchet
 * \date   July 2024
 *********************************************************************/

#pragma once

#include "LoggerUtils.h"
#include "LoggingUtils.h"

/**
 * @brief Base void namespace
 */

#define USE_BASE_NS(ns) using namespace ns
#define START_BASE_NS(ns) namespace ns{
#define END_NS };

#define VOID_NS Void
#define USE_VOID_NS USE_BASE_NS(VOID_NS)
#define START_VOID_NS START_BASE_NS(VOID_NS)
#define END_VOID_NS END_NS

#define USE_VOID_SUB_NS(ns_name) USE_VOID_NS::ns_name
#define START_VOID_SUB_NS(ns_name) START_BASE_NS(VOID_NS::ns_name)

/**
 * @brief Obs builders
 */

#define OBS_BUILDER_NS ObsBuilders
#define USE_OBS_BUILDER_NS USE_VOID_SUB_NS(OBS_BUILDER_NS)
#define START_OBS_BUILDER_NS START_VOID_SUB_NS(OBS_BUILDER_NS)
#define END_OBS_BUILDER_NS END_NS

#define START_OBS_BUILDER_SUB_NS(ns) START_VOID_SUB_NS(OBS_BUILDER_NS::ns)
#define USE_OBS_BUILDER_SUB_NS(ns) USE_VOID_SUB_NS(OBS_BUILDER_NS::ns)

/**
 * @brief Action parsers
 */

#define ACTION_PARSER_NS ActionParsers
#define USE_ACTION_PARSER_NS USE_VOID_SUB_NS(ACTION_PARSER_NS)
#define START_ACTION_PARSER_NS START_VOID_SUB_NS(ACTION_PARSER_NS)
#define END_ACTION_PARSER_NS END_NS

#define START_ACTION_PARSER_SUB_NS(ns) START_VOID_SUB_NS(ACTION_PARSER_NS::ns)
#define USE_ACTION_PARSER_SUB_NS(ns) USE_VOID_SUB_NS(ACTION_PARSER_NS::ns)

/**
 * @brief Loggers
 */

#define LOGGERS_NS Loggers
#define USE_LOGGERS_NS USE_VOID_SUB_NS(LOGGERS_NS)
#define START_LOGGERS_NS START_VOID_SUB_NS(LOGGERS_NS)
#define END_LOGGERS_NS END_NS


/**
 * @brief Rewards
 */

#define REWARDS_NS Rewards
#define USE_REWARDS_NS USE_VOID_SUB_NS(REWARDS_NS)
#define START_REWARDS_NS START_VOID_SUB_NS(REWARDS_NS)
#define END_REWARDS_NS END_NS

#define START_REWARDS_SUB_NS(ns) START_VOID_SUB_NS(REWARDS_NS::ns)
#define USE_REWARDS_SUB_NS(ns) USE_VOID_SUB_NS(REWARDS_NS::ns)

/**
 * @brief States
 */

#define STATES_NS States
#define USE_STATES_NS USE_VOID_SUB_NS(STATES_NS)
#define START_STATES_NS START_VOID_SUB_NS(STATES_NS)
#define END_STATES_NS END_NS

#define START_STATES_SUB_NS(ns) START_VOID_SUB_NS(STATES_NS::ns)
#define USE_STATES_SUB_NS(ns) USE_VOID_SUB_NS(STATES_NS::ns)
