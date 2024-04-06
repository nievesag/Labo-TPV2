#include "FoodSystem.h"

#include "../ecs/Manager.h"

FoodSystem::FoodSystem()
{
}

FoodSystem::~FoodSystem()
{
}

void FoodSystem::initSystem()
{
	cols = 5;
	fils = 4;

	setFruits();
}

void FoodSystem::update()
{
}

void FoodSystem::setFruits()
{

	for (int i = 0; i < cols*fils; i++ ) {

		// add and entity to the manager
			//
		auto e = mngr_->addEntity(ecs::grp::FRUITS);

		auto tr = mngr_->addComponent<Transform>(e);

		tr->width_ = 25;
		tr->height_ = 25;

		// add an Image Component
		//
		auto img = mngr_->addComponent<Image>(e, &sdlutils().images().at("HolaSDL"));

		// miraculous ladybug
		//


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

void FoodSystem::destroyFruit()
{
}

bool FoodSystem::noFruits()
{
    return totalFruits == 0;
}

void FoodSystem::end()
{
    // cambia el estado
    
   
}
