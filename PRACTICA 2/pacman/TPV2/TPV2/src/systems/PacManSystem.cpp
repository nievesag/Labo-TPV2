// This file is part of the course TPV2@UCM - Samir Genaim

#include "PacManSystem.h"

#include "../components/Image.h"
#include "../components/Transform.h"
#include "../components/LivesLeftComponent.h"
#include "../ecs/Manager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../components/IsInmuneComponent.h"

PacManSystem::PacManSystem() :
		pmTR_(nullptr) {
}

PacManSystem::~PacManSystem() {
}

void PacManSystem::initSystem() {

	// create the PacMan entity 
	auto pacman = mngr_->addEntity();

	// lo hace handler (lo que controla el jugador)
	mngr_->setHandler(ecs::hdlr::PACMAN, pacman);

	// para pos inicial de pacman
	auto s = 50.0f;
	auto x = (sdlutils().width() - s) / 2.0f;
	auto y = (sdlutils().height() - s) / 2.0f;

	// le aniade componentes
	pmTR_ = mngr_->addComponent<Transform>(pacman);
	mngr_->addComponent<Image>(pacman, &sdlutils().images().at("pacman"));
	mngr_->addComponent<LivesLeftComponent>(pacman, 3); // inicializa vidas a 3
	mngr_->addComponent<IsInmuneComponent>(pacman, false); // inicializa a no inmune

	// inicializa las cosas
	pmTR_->init(Vector2D(x, y), Vector2D(), s, s, 0.0f);
}

void PacManSystem::update()
{
	// si se pulsa tecla
	if (ih().keyDownEvent()) {

		// girar a la derecha
		if (ih().isKeyDown(SDL_SCANCODE_RIGHT)) 
		{
			// rotacion a 90 grados
			pmTR_->rot_ = pmTR_->rot_ + 90;
			pmTR_->vel_ = pmTR_->vel_.rotate(90);
		}

		// girar izquierda
		else if (ih().isKeyDown(SDL_SCANCODE_LEFT)) 
		{ 
			pmTR_->rot_ = pmTR_->rot_ - 90;
			pmTR_->vel_ = pmTR_->vel_.rotate(-90);
		}

		// aumenta velocidad
		else if (ih().isKeyDown(SDL_SCANCODE_UP)) 
		{ 
			// Add 1.0f to the speed (respecting the limit 3.0f)
			// Recall that speed is the length of the velocity vector
			float speed = std::min(3.0f, pmTR_->vel_.magnitude() + 1.0f);

			// Change the length of velocity vector to 'speed'
			// We need '.rotate(rot)' for the case in which the current speed is 0,
			// so we rotate it to the same direction where the PacMan is looking
			pmTR_->vel_ = Vector2D(0, -speed).rotate(pmTR_->rot_);
		}

		// decrementa velocidad
		else if (ih().isKeyDown(SDL_SCANCODE_DOWN)) 
		{ 
			// Subtract 1.0f to the speed (respecting the limit 0.0f)
			// Recall that speed is the length of the velocity vector
			float speed = std::max(0.0f, pmTR_->vel_.magnitude() - 1.0f);

			// Change the length of velocity vector to 'speed'
			// We need '.rotate(rot)' for the case in which the current speed is 0,
			// so we rotate it to the same direction where the PacMan is looking
			pmTR_->vel_ = Vector2D(0, -speed).rotate(pmTR_->rot_);
		}
	}

	// mueve a pacman
	pmTR_->pos_ = pmTR_->pos_ + pmTR_->vel_;

	// colisiones con los bordes izq/der
	if (pmTR_->pos_.getX() < 0) 
	{
		pmTR_->pos_.setX(0.0f);
		pmTR_->vel_.set(0.0f, 0.0f);
	}
	else if (pmTR_->pos_.getX() + pmTR_->width_ > sdlutils().width()) 
	{
		pmTR_->pos_.setX(sdlutils().width() - pmTR_->width_);
		pmTR_->vel_.set(0.0f, 0.0f);
	}

	// colision con los bordes arriba/abajo
	if (pmTR_->pos_.getY() < 0) 
	{
		pmTR_->pos_.setY(0.0f);
		pmTR_->vel_.set(0.0f, 0.0f);
	}
	else if (pmTR_->pos_.getY() + pmTR_->height_ > sdlutils().height()) 
	{
		pmTR_->pos_.setY(sdlutils().height() - pmTR_->height_);
		pmTR_->vel_.set(0.0f, 0.0f);
	}
}

void PacManSystem::resetGame()
{
	// vidas
	auto livesLeftComponent = mngr_->getComponent<LivesLeftComponent>(pacman);
	livesLeftComponent->resetLives();
}

void PacManSystem::recieve(const Message& m)
{
	switch (m.id) {
	case _m_KILL_PACMAN:
		die();
		break;

	case _m_ROUND_START:
		resetRound();
		break;

	case _m_NEW_GAME:
		resetGame();

	default:
		break;
	}
}

void PacManSystem::die()
{
	// round end
	Message gameOver;

	gameOver.id = _m_GAME_OVER;

	mngr_->send(gameOver, true);
}

void PacManSystem::resetRound()
{
	// reset pos
	auto s = 50.0f;
	auto x = (sdlutils().width() - s) / 2.0f;
	auto y = (sdlutils().height() - s) / 2.0f;

	pmTR_->init(Vector2D(x, y), Vector2D(), s, s, 0.0f);
}