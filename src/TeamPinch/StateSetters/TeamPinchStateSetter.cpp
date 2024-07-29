#include <TeamPinch/TeamPinchUtils.h>
#include <TeamPinch/StateSetters/TeamPinchStateSetter.h>

USE_TEAMPINCH_STATES_NS;

GameState TeamPinchStateSetter::ResetState(Arena* arena)
{
	//Insert your state setter logic here

	return GameState(arena);
}
