#include <Recovery/RecoveryUtils.h>
#include <Recovery/StateSetters/RecoveryStateSetter.h>

USE_RECOVERY_STATES_NS;

GameState RecoveryStateSetter::ResetState(Arena* arena)
{
	//Insert your state setter logic here

	return GameState(arena);
}
