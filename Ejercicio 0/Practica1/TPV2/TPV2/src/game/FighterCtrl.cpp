#include "FighterCtrl.h"

FighterCtrl::FighterCtrl()
{
}

FighterCtrl::~FighterCtrl()
{
}

void FighterCtrl::RotateFighter(Container* o)
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
	}

}
