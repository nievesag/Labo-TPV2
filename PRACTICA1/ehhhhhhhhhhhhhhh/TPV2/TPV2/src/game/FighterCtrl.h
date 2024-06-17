#pragma once

#include "Container.h"
#include "InputComponent.h"
#include "../sdlutils/InputHandler.h" 

class GameObject;
class Vector2D;

class FighterCtrl : public InputComponent {
public:
	FighterCtrl();
	virtual ~FighterCtrl();
	void handleInput(Container* o) override;

private:

	double THURST = 0.2;
	float SPEED_LIMIT = 3.0f;
};


