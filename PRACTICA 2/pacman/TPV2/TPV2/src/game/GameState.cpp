#include "GameState.h"

void GameState::setContext(ecs::Manager* manager)
{
	// para poder acceder al manager desde todos los estados
	mngr_ = manager;
}