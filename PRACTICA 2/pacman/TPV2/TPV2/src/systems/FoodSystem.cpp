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
}

void FoodSystem::update()
{
}

void FoodSystem::setFruits()
{

	// Always use the random number generator provided by SDLUtils
	//
	auto& rand = sdlutils().rand();

	if (ghostCD + lastSpawn < sdlutils().virtualTimer().currTime()
		&& currentGhosts_ < ghostLimit_) {

		lastSpawn = sdlutils().virtualTimer().currTime();

		// add and entity to the manager
				//
		auto e = mngr_->addEntity(ecs::grp::GHOSTS);

		// add a Transform component, and initialise it with random size and position
		//
		auto tr = mngr_->addComponent<Transform>(e);

		// add an Image Component
		//
		auto img = mngr_->addComponent<Image>(e, &sdlutils().images().at("tennis_ball"));

		// image with frames setup
		//
		int width = 40;
		int height = 40;

		tr->width_ = width;
		tr->height_ = height;

		int pos = sdlutils().rand().nextInt(0, 4);

		switch (pos)
		{
		case 0:
			// esquina superior izq 

			tr->pos_.setX(0);
			tr->pos_.setY(0);

			break;
		case 1:
			// esquina superior derecha

			tr->pos_.setX(0);
			tr->pos_.setY(sdlutils().height() - height);	// - height
			break;
		case 2:
			// esquina inferior izquierda

			tr->pos_.setX(sdlutils().width() - width);	// - width
			tr->pos_.setY(0);
			break;
		case 3:
			// esquina inferior derecha

			tr->pos_.setX(sdlutils().width() - width);	// - width
			tr->pos_.setY(sdlutils().height() - height);	// - height
			break;
		default:
			break;
		}

		currentGhosts_++;
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
