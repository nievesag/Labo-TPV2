#include "InmunitySystem.h"
#include "../sdlutils/SDLUtils.h"
#include "../components/IsInmuneComponent.h"
#include "../ecs/Manager.h"
#include "../components/Transform.h"

InmunitySystem::InmunitySystem() {}

InmunitySystem::~InmunitySystem() {}

void InmunitySystem::initSystem() {

	// coge la primera entidad del vector de entidades de tipo PACMAN
	auto pacmanvec = mngr_->getEntities(ecs::hdlr::PACMAN);
	pacman = *pacmanvec.begin(); // es la primera entidad del vector
}

void InmunitySystem::update()
{
	// si actualmente se encuentra en un estado de inmunidad y
	// han pasado 10 segundos desde su inicio:
	// lo apaga enviando un mensaje correspondiente
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

	// si se recibe el mensaje de inicio de inmunidad
	case _m_IMMUNITY_START:
		timeOfActivation = sdlutils().virtualTimer().currTime(); // guarda inicio de inmunidad
		break;

	default:
		break;
	}
}