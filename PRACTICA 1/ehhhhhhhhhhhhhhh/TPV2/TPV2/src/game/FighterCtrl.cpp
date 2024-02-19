#include "FighterCtrl.h"

FighterCtrl::FighterCtrl()
{
}

FighterCtrl::~FighterCtrl()
{
}

void FighterCtrl::handleInput(Container* o)
{

	InputHandler& ihdlr = ih();

	if (ihdlr.keyDownEvent()) {
		if (ihdlr.isKeyDown(SDLK_LEFT)) {
			o->setRotation(o->getRotation() - 5.0f);
		}
		else if (ihdlr.isKeyDown(SDLK_RIGHT)) {
			o->setRotation(o->getRotation() + 5.0f);
		}
		else if (ihdlr.isKeyDown(SDLK_UP)) {
			
			// coge la velocidad del objeto (como coge la referencia se pudede modificar
			// con velo)
			Vector2D& velo = o->getVel();

			//
			velo = velo + Vector2D(0, -1).rotate(o->getRotation()) * THURST;

			// si se pasa del limite normaliza la velocidad
			if (velo.magnitude() > SPEED_LIMIT) {
				velo.normalize() * SPEED_LIMIT;
			}

		}
	}

}
