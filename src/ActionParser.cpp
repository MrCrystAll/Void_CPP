#include "ActionParser.h"
#include <Utils/LoggerUtils.h>

USE_ACTION_PARSER_NS;

DashParser::DashParser()
{
	Action a = {};

	constexpr float
		// Boolean input
		R_B[] = { 0, 1 },

		// Float input (hint: you can add partial inputs here)
		R_F[] = { -1, 0, 1 };

	// TODO: Use std permutations here or whatever			

	// Ground
		for (float steer : R_F) {
			for (float boost : R_B) {
				for (float handbrake : R_B) {
					actions.push_back(
						{
							0, steer, 0, steer, 0, 0, boost, handbrake
						}
					);
				}
			}
	}

	// Aerial
	for (float pitch : R_F) {
		for (float yaw : R_F) {
			for (float roll : R_F) {
				for (float jump : R_B) {
					for (float boost : R_B) {
						// Only need roll for sideflip
						if (jump == 1 && yaw != 0)
							continue;

						// Duplicate with ground
						if (pitch == roll && roll == jump && jump == 0)
							continue;

						// Enable handbrake for potential wavedashes
						float handbrake = (jump == 1) && (pitch != 0 || yaw != 0 || roll != 0);

						actions.push_back(
							{
								0, yaw, pitch, yaw, roll, jump, boost, handbrake
							}
						);
					}
				}
			}
		}
	}

	static std::once_flag onceFlag;
	std::call_once(onceFlag,
		[=] {
			VOID_LOG("DashParser(): Lookup table built, action count: " << actions.size());
		}
	);
}