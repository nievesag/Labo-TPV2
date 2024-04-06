// This file is part of the course TPV2@UCM - Samir Genaim

#include "PacManSystem.h"

#include "../components/Image.h"
#include "../components/ImageWithFrames.h"
#include "../components/Transform.h"
#include "../components/LivesLeftComponent.h"
#include "../ecs/Manager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../components/IsInmuneComponent.h"

PacManSystem::PacManSystem() : pmTR_(nullptr) {}

PacManSystem::~PacManSystem() {}

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
	auto img = mngr_->addComponent<ImageWithFrames>(pacman, &sdlutils().images().at("pacman_spritesheet"), 8, 8);
	mngr_->addComponent<LivesLeftComponent>(pacman, 3); // inicializa vidas a 3
	mngr_->addComponent<IsInmuneComponent>(pacman, false); // inicializa a no inmune

	// inicializa las cosas
	pmTR_->init(Vector2D(x, y), Vector2D(), s, s, 0.0f);

	// image with frames setup
	int width = 40;
	int height = 40;

	pmTR_->width_ = width;
	pmTR_->height_ = height;
	img->setRow(0);
	img->setColFrames(4);
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

	playSound();
}

void PacManSystem::resetGame()
{
	// vidas
	auto livesLeftComponent = mngr_->getComponent<LivesLeftComponent>(mngr_->getHandler(ecs::hdlr::PACMAN));
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
		break;

	case _m_IMMUNITY_END:
		mngr_->getComponent<IsInmuneComponent>(mngr_->getHandler(ecs::hdlr::PACMAN))->setInmune(false);
		break;

	case _m_ROUND_OVER:
		stopSound();
		sdlutils().soundEffects().at("won").play();
		break;

	default:
		break;
	}
}

void PacManSystem::die()
{
	mngr_->getComponent<LivesLeftComponent>(mngr_->getHandler(ecs::hdlr::PACMAN))->updateLives(-1);

	Message gameOver;

	// si aun quedan vidas
	if(mngr_->getComponent<LivesLeftComponent>(mngr_->getHandler(ecs::hdlr::PACMAN))->livesLeft_ > 0)
	{
		gameOver.id = _m_ROUND_OVER;
	}
	else
	{
		gameOver.id = _m_GAME_OVER;
	}
	mngr_->send(gameOver, true);

	stopSound();
	sdlutils().soundEffects().at("death").play();
}

void PacManSystem::playSound()
{
	if (!playing) {
		sdlutils().soundEffects().at("chomp").play(-1);
		playing = true;
	}
}

void PacManSystem::stopSound()
{
	if (playing) {
		sdlutils().soundEffects().at("chomp").pauseChannel();
		playing = false;
	}
}

void PacManSystem::resetRound()
{
	// reset pos
	auto s = 50.0f;
	auto x = (sdlutils().width() - s) / 2.0f;
	auto y = (sdlutils().height() - s) / 2.0f;

	pmTR_->init(Vector2D(x, y), Vector2D(), s, s, 0.0f);
}