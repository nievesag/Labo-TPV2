#include "GhostSystem.h"

#include <algorithm>
#include "../components/Image.h"
#include "../components/Points.h"
#include "../components/StarMotion.h"
#include "../components/Transform.h"
#include "../ecs/Manager.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"
#include "GameCtrlSystem.h"

GhostSystem::GhostSystem()
{
}

GhostSystem::~GhostSystem()
{
}

void GhostSystem::initSystem()
{
	ghostLimit_ = 4;
	currentGhosts_ = 0;
	
}

void GhostSystem::update()
{
	// no va porque no esta adaptado
	auto currTime = sdlutils().currRealTime();
	auto ghosts = mngr_->getEntities(ecs::grp::GHOSTS);
	auto n = ghosts.size();

	for (auto i = 0u; i < n; i++) {
		auto tr = mngr_->getComponent<Transform>(ghosts[i]);
		auto starmotion = mngr_->getComponent<StarMotion>(ghosts[i]);

		if (starmotion->shouldUpdate(currTime)) {

			// rotate it
			tr->rot_ += starmotion->rot_;

			// resize it
			tr->width_ *= 0.95f;
			tr->height_ *= 0.95f;

			// check if it should die
			if (tr->width_ < starmotion->sizeLimit_
				|| tr->height_ < starmotion->sizeLimit_) {
				mngr_->setAlive(ghosts[i], false);
				currentGhosts_--;
			}
		}
	}
}

void GhostSystem::generateGhost()
{
	// Always use the random number generator provided by SDLUtils
	//
	auto& rand = sdlutils().rand();

	//auto limit = std::min( //
	//	static_cast<unsigned int>(n), //
	//	starsLimit_ - currNumOfStars_);


	for (auto i = 0u; i < ghostLimit_; i++) {

		// add and entity to the manager
		//
		auto e = mngr_->addEntity(ecs::grp::GHOSTS);

		// add a Transform component, and initialise it with random
		// size and position
		//
		auto tr = mngr_->addComponent<Transform>(e);
		auto s = rand.nextInt(50, 100);
		auto x = rand.nextInt(0, sdlutils().width() - s);
		auto y = rand.nextInt(0, sdlutils().height() - s);
		tr->init(Vector2D(x, y), Vector2D(), s, s, 0.0f);

		// add an Image Component
		//
		mngr_->addComponent<Image>(e, &sdlutils().images().at("star"));

		// add a StarMotion component to resize/rotate the star
		//
		//auto motion = mngr_->addComponent<StarMotion>(e);

		//motion->rot_ = rand.nextInt(5, 10);
		//motion->sizeLimit_ = rand.nextInt(2, 10);
		//motion->updateFreq_ = rand.nextInt(20, 100);

		//auto pts = mngr_->addComponent<Points>(e);
		//pts->points_ = rand.nextInt(1, 5);

		currentGhosts_++;
	}

}

void GhostSystem::killPacman()
{
	//

	std::cout << "MUEREEEEEEEEEEEEE" << std::endl;
}

void GhostSystem::recieve(const Message& m)
{
	switch (m.id) {
	case _m_KILL_PACMAN:
		killPacman();
		break;
	default:
		break;
	}
}
