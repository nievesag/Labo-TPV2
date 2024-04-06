// This file is part of the course TPV2@UCM - Samir Genaim

#include "GameCtrlSystem.h"

#include "../components/Points.h"
#include "../ecs/Manager.h"
#include "../sdlutils/InputHandler.h"
#include "StarsSystem.h"
#include "../game/Game.h"

GameCtrlSystem::GameCtrlSystem() :
		score_() {
	// TODO Auto-generated constructor stub
	

}

GameCtrlSystem::~GameCtrlSystem() {
	// TODO Auto-generated destructor stub
}

void GameCtrlSystem::initSystem() {
	score_ = 0;
}

void GameCtrlSystem::update() {}

void GameCtrlSystem::recieve(const Message &m) {
	switch (m.id) {
	case _m_GAME_OVER:
		Game::instance()->setState(Game::GAMEOVER);
		break;
	case _m_ROUND_OVER:
		Game::instance()->setState(Game::NEWROUND);
		break;

	default:
		break;
	}
}
