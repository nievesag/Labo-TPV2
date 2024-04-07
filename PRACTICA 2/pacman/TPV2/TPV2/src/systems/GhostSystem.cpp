#include "GhostSystem.h"

#include <algorithm>
#include "../components/Image.h"
#include "../components/ImageWithFrames.h"
#include "../components/Transform.h"
#include "../ecs/Manager.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"
#include "GameCtrlSystem.h"
#include "../components/IsInmuneComponent.h"

GhostSystem::GhostSystem() {}

GhostSystem::~GhostSystem() {}

void GhostSystem::initSystem()
{
	ghostLimit_ = 7;
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
	auto& rand = sdlutils().rand();

	if (ghostCD + lastSpawn < sdlutils().virtualTimer().currTime() &&
		currentGhosts_ < ghostLimit_ && 
		!(mngr_->getComponent<IsInmuneComponent>(mngr_->getHandler(ecs::hdlr::PACMAN))->isInmune)) {

		lastSpawn = sdlutils().virtualTimer().currTime();

		// add and entity to the manager
		auto e = mngr_->addEntity(ecs::grp::GHOSTS);

		// add a Transform component, and initialise it with random size and position
		auto tr = mngr_->addComponent<Transform>(e);

		// add an Image Component
		auto img = mngr_->addComponent<ImageWithFrames>(e, &sdlutils().images().at("pacman_spritesheet"), 8, 8); //mngr_->addComponent<Image>(e, &sdlutils().images().at("tennis_ball"));

		// image with frames setup
		int width = 40;
		int height = 40;

		tr->width_ = width;
		tr->height_ = height;

		int pos = sdlutils().rand().nextInt(0, 4);

		switch (pos)
		{
		// esquina superior izq 
		case 0:
			tr->pos_.setX(0);
			tr->pos_.setY(0);

			img->setRow(4);
			img->setColFrames(8);
			break;

		// esquina superior derecha
		case 1:
			tr->pos_.setX(0);
			tr->pos_.setY(sdlutils().height() - height);	// - height

			img->setRow(5);
			img->setColFrames(8);
			break;

		// esquina inferior izquierda
		case 2:
			tr->pos_.setX(sdlutils().width() - width);	// - width
			tr->pos_.setY(0);

			img->setRow(6);
			img->setColFrames(8);
			break;

		case 3:
			// esquina inferior derecha
			tr->pos_.setX(sdlutils().width() - width);	// - width
			tr->pos_.setY(sdlutils().height() - height);	// - height

			img->setRow(7);
			img->setColFrames(8);
			break;

		default:
			break;
		}

		currentGhosts_++;
	}
}

void GhostSystem::moveGhosts()
{
	auto ghosts = mngr_->getEntities(ecs::grp::GHOSTS);

	// recorre todos los ghosts
	for (auto& g : ghosts) {

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

		gt->pos_ = gt->pos_ + gt->vel_;

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

void GhostSystem::resetGhosts()
{
	destroyGhosts();
	currentGhosts_ = 0;
}

void GhostSystem::destroyGhosts()
{
	for (auto& g : mngr_->getEntities(ecs::grp::GHOSTS)) {
		mngr_->setAlive(g, false);
	}
}

void GhostSystem::changeGhosts()
{
	// HOLAAA INEESSSS AQUI ESS
	// si pacman es inmune en ese momento cambia la image de los fantasmas
	if(mngr_->getComponent<IsInmuneComponent>(mngr_->getHandler(ecs::hdlr::PACMAN))->isInmune)
	{

		for (auto& g : mngr_->getEntities(ecs::grp::GHOSTS)) {

			auto img = mngr_->getComponent<ImageWithFrames>(g);

			img->setXoffset(6);
			img->setRow(3);
			img->setColFrames(2);

		}
	}
	// vuelven a la normalidad
	else
	{
		for (auto& g : mngr_->getEntities(ecs::grp::GHOSTS)) {

			auto img = mngr_->getComponent<ImageWithFrames>(g);


			int color = sdlutils().rand().nextInt(4, 7);

			img->setRow(color);
			img->reset();
			img->setColFrames(8);
		}
		
	}
}

void GhostSystem::destroyGhost(ecs::entity_t ghost)
{
	mngr_->setAlive(ghost, false);
}

void GhostSystem::recieve(const Message& m)
{
	switch (m.id) {
	case _m_ROUND_START:
		resetGhosts(); // resetea contador de generacion de fantasmas
		break;

	case _m_NEW_GAME:
		resetGhosts(); // resetea contador de generacion de fantasmas
		break;

	case _m_IMMUNITY_START:
		changeGhosts(); // cambia la image de los fantasmas al ser pacman inmune
		break;

	case _m_IMMUNITY_END:
		changeGhosts(); // cambia la image de los fantasmas al dejar de ser pacman inmune
		break;

	case _m_EAT_GHOST:
		destroyGhost(m.eat_ghost_data.e);
		break;

	default:
		break;
	}
}