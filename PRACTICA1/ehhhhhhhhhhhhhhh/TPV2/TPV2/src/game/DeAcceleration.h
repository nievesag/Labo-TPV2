#pragma once


#include "../game/PhysicsComponent.h"
#include "../game/Container.h"

class DeAcceleration : public PhysicsComponent {

public:

	DeAcceleration();
	~DeAcceleration();
	void update(Container *o);

private:
	float deceleration = 0.992f;

};