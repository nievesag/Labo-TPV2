#include "RunningState.h"
#include "../systems/PacManSystem.h"
#include "../systems/CollisionsSystem.h"
#include "../systems/GhostSystem.h"
#include "../systems/FoodSystem.h"
#include "../systems/InmunitySystem.h"


RunningState::RunningState()
{


}

RunningState::~RunningState()
{

	delete pacmanSys_;
	pacmanSys_ = nullptr;
}

void RunningState::leave()
{
	

}

void RunningState::update()
{

	// se actualizan los sistemas
	pacmanSys_->update();
	collisionSys_->update();
	ghostSys_->update();
	foodSys_->update();
	inmunitySys_->update();

	mngr_->refresh();

}

void RunningState::enter()
{
	pacmanSys_ = mngr_->getSystem<PacManSystem>();
	collisionSys_ = mngr_->getSystem<CollisionsSystem>();
	ghostSys_ = mngr_->getSystem<GhostSystem>();
	foodSys_ = mngr_->getSystem<FoodSystem>();
	inmunitySys_ = mngr_->getSystem<InmunitySystem>();


	pacmanSys_->initSystem();
	collisionSys_->initSystem();
	ghostSys_->initSystem();
	foodSys_->initSystem();
	inmunitySys_->initSystem();
}
