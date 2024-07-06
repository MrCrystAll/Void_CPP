/*****************************************************************//**
 * \file   GroundDoubleTapState.h
 * \brief  Overfitting ground double tap setup
 * 
 * \author Mathieu Suchet
 * \date   July 2024
 *********************************************************************/

#pragma once
#include "RLGymSim_CPP\Utils\StateSetters\StateSetter.h"
using namespace RLGSC;
class GroundDoubleTapState :
    public RLGSC::StateSetter
{
public:
    virtual GameState ResetState(Arena* arena);
};

