/*****************************************************************//**
 * \file   carball_handling.h
 * \brief  Handling of all the carball related stuff
 * 
 * \author Mathieu Suchet
 * \date   July 2024
 *********************************************************************/

#include <Replays/csv.h>
#include <Utils/LoggerUtils.h>
#include <Replays/ReplayUtils.h>
#include <iostream>

#pragma once

START_REPLAY_NS

int CallCarball(std::string replayPath, std::string output_path = "replays/tmp");

END_REPLAY_NS