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
	
	generateGhost();

	moveGhosts();

}

void GhostSystem::generateGhost()
{
	// Always use the random number generator provided by SDLUtils
	//
	auto& rand = sdlutils().rand();

	//auto limit = std::min( //
	//	static_cast<unsigned int>(n), //
	//	starsLimit_ - currNumOfStars_);


	for (auto i = currentGhosts_; i < ghostLimit_; i++) {

		// add and entity to the manager
		//
		auto e = mngr_->addEntity(ecs::grp::GHOSTS);

		// add a Transform component, and initialise it with random size and position
		//
		auto tr = mngr_->addComponent<Transform>(e);

		int pos = sdlutils().rand().nextInt(0, 4);
		int x, y, s;

		switch (pos)
		{
		case 0:
			x = 0;
			y = 0;
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		default:
			break;
		}
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

void GhostSystem::moveGhosts()
{
	auto ghosts = mngr_->getEntities(ecs::grp::GHOSTS);

	// recorre todos los ghosts
	for (auto& g : ghosts) {

		// coge el Transform de la entidad
		auto gt = mngr_->getComponent<Transform>(g);

		// probabilidad random de que se actualice el vector
		int prob = sdlutils().rand().nextInt(1, 500);

		if (prob == 1) {

			// coge el transform del pacman
			auto PM = mngr_->getHandler(ecs::hdlr::PACMAN);
			auto posPM = mngr_->getComponent<Transform>(PM);

			// modifica la velocidad del ghost con respecto de la del pacman
			gt->vel_ = (posPM->pos_ - gt->pos_).normalize() * 1.1f;

		}


		// check left/right borders
		if (gt->pos_.getX() < 0) {
			gt->pos_.setX(0.0f);
			gt->vel_.set(0.0f, 0.0f);
		}
		else if (gt->pos_.getX() + gt->width_ > sdlutils().width()) {
			gt->pos_.setX(sdlutils().width() - gt->width_);
			gt->vel_.set(0.0f, 0.0f);
		}

		// check upper/lower borders
		if (gt->pos_.getY() < 0) {
			gt->pos_.setY(0.0f);
			gt->vel_.set(0.0f, 0.0f);
		}
		else if (gt->pos_.getY() + gt->height_ > sdlutils().height()) {
			gt->pos_.setY(sdlutils().height() - gt->height_);
			gt->vel_.set(0.0f, 0.0f);
		}
		
	}
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
