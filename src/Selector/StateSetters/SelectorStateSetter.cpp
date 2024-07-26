#include <Selector/SelectorUtils.h>
#include <Selector/StateSetters/SelectorStateSetter.h>

USE_SELECTOR_STATES_NS;

GameState SelectorStateSetter::ResetState(Arena* arena)
{
	//Insert your state setter logic here

	return GameState(arena);
}
