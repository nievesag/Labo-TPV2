#include "ShowAtOppositeSide.h"

#include "../game/Container.h"
#include "../sdlutils/SDLUtils.h"

ShowAtOppositeSide::ShowAtOppositeSide()
{
}

ShowAtOppositeSide::~ShowAtOppositeSide()
{
}

void ShowAtOppositeSide::update(Container* o)
{

	Vector2D& pos = o->getPos();

	// DERECHA
	if (o->getPos().getX() > sdlutils().width() + o->getHeight()) {
		pos = Vector2D(0 - o->getHeight(), pos.getY());
	}
	// IZQUIERDA
	else if (o->getPos().getX() < 0 - o->getHeight()) {
		pos = Vector2D(sdlutils().width() + o->getHeight(), pos.getY());
	}


	//// ARRIBA
	//else if (o->getPos().getY() < 0) {
	//	pos = Vector2D(pos.getX(), sdlutils().height());
	//}
	//// ABAJO
	//else if (o->getPos().getY() > o->getHeight()) {
	//	pos = Vector2D(pos.getX(), 0);
	//}

}
