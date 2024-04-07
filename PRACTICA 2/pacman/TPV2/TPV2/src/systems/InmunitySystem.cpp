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
	pacman = *pacmanvec.begin();
}

void InmunitySystem::update()
{
	// si actualmente se encuentra en un estado de inmunidad y hayan pasado 10
	// segundos desde que su inicio, lo apaga enviando un mensaje correspondiente

	mngr_->getComponent<Transform>(pacman);

	if(mngr_->getComponent<IsInmuneComponent>(pacman)->getIsInmune() &&
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
		timeOfActivation = sdlutils().virtualTimer().currTime();
		break;

	default:
		break;
	}
}