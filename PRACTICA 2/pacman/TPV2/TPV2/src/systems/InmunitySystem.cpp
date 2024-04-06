#include "InmunitySystem.h"
#include "../sdlutils/SDLUtils.h"
#include "../components/IsInmuneComponent.h"
#include "../ecs/Manager.h"

InmunitySystem::InmunitySystem() {}

InmunitySystem::~InmunitySystem() {}

void InmunitySystem::initSystem() {}

void InmunitySystem::update()
{
	// si actualmente se encuentra en un estado de inmunidad y hayan pasado 10
	// segundos desde que su inicio, lo apaga enviando un mensaje correspondiente
	if(mngr_->getComponent<IsInmuneComponent>(pacman)->isInmune &&
		timeOfActivation + maxInmunityDuration < sdlutils().virtualTimer().currTime())
	{
		Message m;
		m.id = _m_IMMUNITY_END;
		mngr_->send(m, true);
	}
}

void InmunitySystem::recieve(const Message& m)
{
	switch (m.id) {
	case _m_IMMUNITY_START:
		// si no se encuentra en periodo de inmunidad empieza el contador
		if(!mngr_->getComponent<IsInmuneComponent>(pacman)->isInmune)
		{
			timeOfActivation = sdlutils().virtualTimer().currTime();
		}
		break;

	default:
		break;
	}
}
