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
	ghostLimit_ = 10;
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

	for (auto i = currentGhosts_; i < ghostLimit_; i++) {

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

		//std::cout << "existe " << mngr_->getComponent<Transform>(g)->pos_ << std::endl;

		// coge el Transform de la entidad
		auto gt = mngr_->getComponent<Transform>(g);

		// probabilidad random de que se actualice el vector
		int prob = sdlutils().rand().nextInt(1, 100);

		if (prob == followChance) {

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
