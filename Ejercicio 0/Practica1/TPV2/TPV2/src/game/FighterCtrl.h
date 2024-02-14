#pragma once

#include "Container.h"
#include "InputComponent.h"

class GameObject;
class Vector2D;

class FighterCtrl : public InputComponent {
public:
	FighterCtrl();
	virtual ~FighterCtrl();
	void RotateFighter(Container* o);
	void handleInput(Container* o) override;

private:
};


