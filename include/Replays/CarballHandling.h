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

/// <summary>
/// Calls carball and returns the return code, writes the results to a specific path
/// </summary>
/// <param name="replayPath">Path to replay file</param>
/// <param name="output_path">The file to write the result to</param>
/// <returns>Return code of the carball call</returns>
int CallCarball(std::string replayPath, std::string output_path = DEFAULT_CARBALL_RESULT_PATH);

END_REPLAY_NS