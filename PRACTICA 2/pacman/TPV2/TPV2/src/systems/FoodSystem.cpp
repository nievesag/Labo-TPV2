#include "FoodSystem.h"

#include "../ecs/Manager.h"
#include "../game/Game.h"
#include "../components/IsMiraculousComponent.h"
#include "../components/ImageWithFrames.h"

FoodSystem::FoodSystem() {}

FoodSystem::~FoodSystem() {}

void FoodSystem::initSystem()
{
	cols = 5;
	fils = 4;
	setFruits(); // coloca las frutas
}

void FoodSystem::update() 
{
	// en cada frame se comprueba si alguna fruta debe transformarse
	miraculousFruitManager();
}

void FoodSystem::recieve(const Message& m)
{
	switch (m.id) {
	case _m_EAT_FRUIT:
		destroyFruit(m.eat_fruit_data.e); // destruye la fruta al comersela pacman
		break;

	case _m_ROUND_START:
		resetFruits(); // vuelve a colocar las frutas al inicio de ronda
		break;

	case _m_NEW_GAME:
		resetFruits(); // vuelve a colocar las frutas al inicio de partida
		break;

	case _m_PACMAN_FOOD_COLLISION:
		foodCollision(m.eat_fruit_data.e);
		break;

	default:
		break;
	}
}

void FoodSystem::setFruits()
{
	// crea las frutas en un grid
	for (int i = 0; i < cols*fils; i++ ) 
	{
		// add and entity to the manager
		auto e = mngr_->addEntity(ecs::grp::FRUITS);

		// aniade transform
		auto tr = mngr_->addComponent<Transform>(e);

		tr->width_ = 25;
		tr->height_ = 25;

		// add an Image Component
		auto img = mngr_->addComponent<ImageWithFrames>(e, &sdlutils().images().at("pacman_spritesheet"), 8, 8);

		// ---
		// A UNA DE CADA 10 FRUTAS LE ASIGNAS EL COMPONENTE MIRACULOUS
		// add component miraculous
		int rnd = sdlutils().rand().nextInt(0, 1000);

		if (rnd < isMiraculousChance) 
		{
			auto iM = mngr_->addComponent<IsMiraculousComponent>(e);
			iM->MiraculousTransformation();
		}
		// ---

		// grid
		float cellx = (offsetX - tr->width_) / 2;	// distancia por celda en x
		float celly = (offsetY - tr->height_) / 2;	// distancia por celda en y

		tr->pos_.set(cellx + (offsetX * (i % cols)),	// posicion de la celda en x
					 celly + (offsetY * (i / cols)));	// posicion de la celda en y

		img->setRow(1);
		img->setCol(4);
		img->setColFrames(1);
		img->setXoffset(6);
	}
}

void FoodSystem::destroyFruit(ecs::entity_t fruit)
{
	// mata la fruta con la que haya colisionado pacman
	mngr_->setAlive(fruit, false);

	// si no quedan frutas (te acabas de comer la ultima) -> has ganado
	if (noFruits()) 
	{
		// round end
		Message wonGame;
		wonGame.id = _m_WIN_GAME;
		mngr_->send(wonGame, true);
	}
}

bool FoodSystem::noFruits()
{
	// devuelve true si la cantidad de entidades en el grupo fruits es menor o igual a 1
	auto fruits = mngr_->getEntities(ecs::grp::FRUITS);
    return fruits.size() <= 1;
}

void FoodSystem::resetFruits()
{
	// resetea las frutas en panatalla y los contadores
	for (auto& f : mngr_->getEntities(ecs::grp::FRUITS)) {
		destroyFruit(f);
	}
	setFruits();
}

void FoodSystem::miraculousFruitManager()
{
	// recorre el grupo de todas las frutas y actualiza las miraculousas
	for (auto& e : mngr_->getEntities(ecs::grp::FRUITS)) {

		// coge el componente de miracoulosidad de la fruta
		auto miracleCmp = mngr_->getComponent<IsMiraculousComponent>(e);

		// para cambiar la imagen a PERA
		auto img = mngr_->getComponent<ImageWithFrames>(e);

		// si tiene el componente (solo lo tienen ciertas frutas)
		if (miracleCmp != nullptr) {

			// si isMiraculous true
			if (miracleCmp->isMiraculous) 
			{
				// si el momento en el que se activo + la duracion max es menor que current time ->
				// se acabo el tiempo de miracoulosidad
				if (miracleCmp->timeOfActivation + miracleCmp->MDuration < sdlutils().virtualTimer().currTime()) {

					// deja de ser milagrosa
					miracleCmp->isMiraculous = false;

					// poner imagen normal
					img->setXoffset(6);
				}
			}

			// si isMiraculous true
			else 
			{
				// si el momento en el que se activo + la frecuencia de spawneo max es menor que el tiempo actual ->
				// toca transformar fruta en milagrosa
				if (miracleCmp->timeOfActivation + miracleCmp->NFrecuency < sdlutils().virtualTimer().currTime()) {

					// activa la transformacion en esa fruta
					miracleCmp->MiraculousTransformation();

					// poner imagen de pera
					img->setXoffset(7);
				}
			}
		}
	}
}

void FoodSystem::foodCollision(ecs::entity_t fruit)
{
	// manda mensaje de fruta comida
	Message f;
	f.id = _m_EAT_FRUIT;
	f.eat_fruit_data.e = fruit;
	mngr_->send(f);

	// PARA ADMINISTRAR INMUNIDAD
	if (mngr_->getComponent<IsMiraculousComponent>(fruit) != nullptr && mngr_->isAlive(fruit))
	{
		// si la fruta con la que colisiones es miracoulosa empieza la inmunidad de pacman
		if (mngr_->getComponent<IsMiraculousComponent>(fruit)->isMiraculous)
		{
			Message m;
			m.id = _m_IMMUNITY_START;
			mngr_->send(m);
		}
	}
}