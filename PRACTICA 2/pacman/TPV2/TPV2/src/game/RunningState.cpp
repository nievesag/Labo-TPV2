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


RunningState::RunningState()
{}

RunningState::~RunningState()
{}

void RunningState::enter()
{
	pacmanSys_ = mngr_->getSystem<PacManSystem>();
	collisionSys_ = mngr_->getSystem<CollisionsSystem>();
	ghostSys_ = mngr_->getSystem<GhostSystem>();
	foodSys_ = mngr_->getSystem<FoodSystem>();
	inmunitySys_ = mngr_->getSystem<InmunitySystem>();
	gameCtrlSys_ = mngr_->getSystem<GameCtrlSystem>();


	//pacmanSys_->initSystem();
	//collisionSys_->initSystem();
	//ghostSys_->initSystem();
	//foodSys_->initSystem();
	//inmunitySys_->initSystem();
}

void RunningState::update()
{
	// move to pause if P pressed
	if (ih().keyDownEvent() && ih().isKeyDown(SDL_SCANCODE_P)) {
		Game::instance()->setState(Game::PAUSED);
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

void RunningState::leave()
{

}