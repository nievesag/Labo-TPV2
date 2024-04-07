#include "RunningState.h"

// input handler
#include "../sdlutils/InputHandler.h"

// game
#include "Game.h"

// sistemas
#include "../systems/PacManSystem.h"
#include "../systems/CollisionsSystem.h"
#include "../systems/GhostSystem.h"
#include "../systems/FoodSystem.h"
#include "../systems/InmunitySystem.h"

RunningState::RunningState() {}

RunningState::~RunningState() {}

void RunningState::enter()
{
	pacmanSys_ = mngr_->getSystem<PacManSystem>();
	collisionSys_ = mngr_->getSystem<CollisionsSystem>();
	ghostSys_ = mngr_->getSystem<GhostSystem>();
	foodSys_ = mngr_->getSystem<FoodSystem>();
	inmunitySys_ = mngr_->getSystem<InmunitySystem>();
	gameCtrlSys_ = mngr_->getSystem<GameCtrlSystem>();
}

void RunningState::update()
{
	// move to pause if P pressed
	if (ih().keyDownEvent() && ih().isKeyDown(SDL_SCANCODE_P)) {

		// va al estado de pausa
		Game::instance()->setState(Game::PAUSED);

		// message
		Message m;
		m.id = _m_PAUSE_GAME;
		mngr_->send(m, true);

		return;
	}

	// se actualizan los sistemas
	pacmanSys_->update();
	collisionSys_->update();
	ghostSys_->update();
	foodSys_->update();
	inmunitySys_->update();
	gameCtrlSys_->update();
}

void RunningState::leave() {}