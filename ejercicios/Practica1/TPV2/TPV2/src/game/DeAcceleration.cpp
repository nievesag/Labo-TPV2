#include "DeAcceleration.h"

DeAcceleration::DeAcceleration()
{
}

DeAcceleration::~DeAcceleration()
{
}

void DeAcceleration::update(Container* o)
{
	Vector2D& velo = o->getVel();
	velo = velo * deceleration;

}
