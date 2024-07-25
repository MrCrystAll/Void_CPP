/*****************************************************************//**
 * \file   DoubleTapState.h
 * \brief  Overfitting and random double tap state setters
 * 
 * \author Mathieu Suchet
 * \date   July 2024
 *********************************************************************/

#pragma once

#include "Utils/DoubleTapUtils.h"
#include "RLGymSim_CPP/Utils/StateSetters/StateSetter.h"

START_STATES_DT_NS

class OverfittingDoubleTapSetter : public RLGSC::StateSetter {
public:
	virtual RLGSC::GameState ResetState(Arena* arena);
};

class DoubleTapState: public RLGSC::StateSetter
{
public:

	virtual RLGSC::GameState ResetState(Arena* arena);

	struct PhysObjVariance {
		Vec posVariance = Vec(400.0f, 0.0f, 200.0f);
		Vec velVariance = Vec(400.0f, 400.0f, 100.0f);
		Angle orientVariance = Angle(2.0f, 2.0f, 2.0f);
		Vec angVelVariance = Vec(100.0f, 100.0f, 100.0f);
	};

	struct DoubleTapStateArgs {
		bool bothSides = false;

		PhysObjVariance carVariance;
		PhysObjVariance ballVariance;

	};

	DoubleTapState(DoubleTapStateArgs args): config(args) {};

private:
	DoubleTapStateArgs config;
};
END_DT_NS