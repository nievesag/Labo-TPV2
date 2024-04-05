#include "RunningState.h"
#include "../systems/PacManSystem.h"


RunningState::RunningState()
{


}

RunningState::~RunningState()
{

	delete pacmanSYS;
	pacmanSYS = nullptr;
}

void RunningState::leave()
{


}

void RunningState::update()
{

	// se actualizan los sistemas
	pacmanSYS->update();

}

void RunningState::enter()
{
	pacmanSYS = new PacManSystem();
	pacmanSYS->initSystem();
}
