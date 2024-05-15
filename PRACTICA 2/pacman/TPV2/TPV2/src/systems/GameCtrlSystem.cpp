// This file is part of the course TPV2@UCM - Samir Genaim

#include "GameCtrlSystem.h"

#include "../ecs/Manager.h"
#include "../sdlutils/InputHandler.h"
#include "../game/Game.h"
#include "../sdlutils/SDLUtils.h"

GameCtrlSystem::GameCtrlSystem() {}

GameCtrlSystem::~GameCtrlSystem() {}

void GameCtrlSystem::initSystem() {}

void GameCtrlSystem::update() {}

void GameCtrlSystem::recieve(const Message &m)
{
	// controla mensajes de paso de estados
	switch (m.id) {
	case _m_GAME_OVER:
		Game::instance()->setState(Game::GAMEOVER);
		break;

	case _m_ROUND_OVER:
		Game::instance()->setState(Game::NEWROUND);
		break;

	case _m_WIN_GAME:
		Game::instance()->setState(Game::GAMEOVER);
		break;

	case _m_EAT_FRUIT:
		//incrScore();
		break;

	default:
		break;
	}
}