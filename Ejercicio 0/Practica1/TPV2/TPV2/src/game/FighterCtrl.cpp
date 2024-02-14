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
	if (SDLK_LEFT) {
		o->setRotation(o->getRotation() + 5.0f);
	}
	else if (SDLK_RIGHT) {
		o->setRotation(o->getRotation() - 5.0f);
	}
}
