#include "FoodSystem.h"

#include "../ecs/Manager.h"
#include "../game/Game.h"
#include "../components/IsMiraculousComponent.h"

FoodSystem::FoodSystem() {}

FoodSystem::~FoodSystem() {}

void FoodSystem::initSystem()
{
	cols = 5;
	fils = 4;

	setFruits();
}

void FoodSystem::update()
{

}

void FoodSystem::recieve(const Message& m)
{
	switch (m.id) {
	case _m_EAT_FRUIT:
		destroyFruit(m.eat_fruit_data.e);
		break;

	case _m_ROUND_START:
		resetFruits();
		break;

	case _m_NEW_GAME:
		resetFruits();
		break;

	default:
		break;
	}
}

void FoodSystem::setFruits()
{
	for (int i = 0; i < cols*fils; i++ ) {

		// add and entity to the manager
	
		auto e = mngr_->addEntity(ecs::grp::FRUITS);

		auto tr = mngr_->addComponent<Transform>(e);

		tr->width_ = 25;
		tr->height_ = 25;

		// add an Image Component
		auto img = mngr_->addComponent<Image>(e, &sdlutils().images().at("HolaSDL"));

		// A UNA DE CADA 10 FRUTAS LE ASIGNAS EL COMPONENTE MIRACULOUS
		// add component miraculous
		int rnd = sdlutils().rand().nextInt(0, 1000);

		if (rnd < isMiraculousChance) {

			auto iM = mngr_->addComponent<IsMiraculousComponent>(e);
			iM->MiraculousTransformation();
		}

		// grid
		float cellx = (offsetX - tr->width_) / 2;	// distancia por celda en x
		float celly = (offsetY - tr->height_) / 2;	// distancia por celda en y

		tr->pos_.set(cellx + (offsetX * (i % cols)),	// posicion de la celda en x
					 celly + (offsetY * (i / cols)));	// posicion de la celda en y
	}
}

void FoodSystem::updateState()
{

}

void FoodSystem::destroyFruit(ecs::entity_t fruit)
{
	mngr_->setAlive(fruit, false);

	auto fruits = mngr_->getEntities(ecs::grp::FRUITS);

	std::cout << fruits.size() << std::endl;

	if (noFruits()) {

		// round end
		Message roundOver;

		roundOver.id = _m_ROUND_OVER;

		mngr_->send(roundOver, true);
	}
}

bool FoodSystem::noFruits()
{
	auto fruits = mngr_->getEntities(ecs::grp::FRUITS);

    return fruits.size() <= 1;
}

void FoodSystem::resetFruits()
{
	// resetea las frutas en panatalla y los contadores
}