/*****************************************************************//**
 * \file   ReplayUtils.h
 * \brief  Utils for the replays
 * 
 * \author Mathieu Suchet
 * \date   July 2024
 *********************************************************************/

#pragma once

#include <Utils/VoidUtils.h>
#include <RLGymSim_CPP/Utils/BasicTypes/Lists.h>

#define REPLAY_NS Replays
#define START_REPLAY_NS START_VOID_SUB_NS(REPLAY_NS)
#define END_REPLAY_NS END_NS
#define USE_REPLAY_NS USE_VOID_SUB_NS(REPLAY_NS)

#define START_REPLAY_SUB_NS(ns) START_VOID_SUB_NS(REPLAY_NS::ns)
#define USE_REPLAY_SUB_NS(ns) USE_VOID_SUB_NS(REPLAY_NS::ns)

#define DEFAULT_CARBALL_RESULT_PATH "replays/tmp"

START_REPLAY_NS

std::string GetTimePath();
std::vector<float> AerialInputs(Vec angVelStart, Vec angVelEnd, RotMat rotMatStart, RotMat rotMatEnd, float dt, bool isFlipping = false);

END_REPLAY_NS


