#include "MissileUtils.h"

#include "src/sdlutils/SDLUtils.h"
#include "src/ecs/Manager.h"
#include "src/components/Image.h"
#include "src/components/Transform.h"
#include "src/game/ImageRenderer.h"
#include "src/components/Follow.h"
#include "src/game/Game.h"

MissileUtils::MissileUtils()
	: mngr(Game::instance()->getMngr())
{

}

void MissileUtils::create_missiles(int n)
{

}

void MissileUtils::remove_all_missiles()
{

}