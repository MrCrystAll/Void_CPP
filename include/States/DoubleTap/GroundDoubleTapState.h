#pragma once
#include "RLGymSim_CPP\Utils\StateSetters\StateSetter.h"
using namespace RLGSC;
class GroundDoubleTapState :
    public RLGSC::StateSetter
{
public:
    virtual GameState ResetState(Arena* arena);
};

