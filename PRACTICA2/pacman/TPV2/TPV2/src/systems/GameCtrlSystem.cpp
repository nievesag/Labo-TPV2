// This file is part of the course TPV2@UCM - Samir Genaim

#include "GameCtrlSystem.h"

#include "../ecs/Manager.h"
#include "../game/Game.h"
#include "../sdlutils/InputHandler.h"
#include "../components/Points.h"
#include "../sdlutils/SDLUtils.h"

GameCtrlSystem::GameCtrlSystem() {}

GameCtrlSystem::~GameCtrlSystem() {}

void GameCtrlSystem::initSystem() {}

void GameCtrlSystem::update() {}

void GameCtrlSystem::recieve(const Message &m)
{
	// controla mensajes de paso de estados
	switch (m.id) {
	case _m_GAME_OVER:{
		Game::instance()->setState(Game::GAMEOVER);
		setScore(0);
	}
		break;

	case _m_ROUND_OVER: {
		Game::instance()->setState(Game::NEWROUND);
		setScore(0);
	}
		break;

	case _m_WIN_GAME: {
		Game::instance()->setState(Game::GAMEOVER);
		setScore(0);
	}
		break;

	case _m_NEW_GAME:
		setScore(0);
		break;

	case _m_EAT_FRUIT:
		incrScore(mngr_->getComponent<Points>(m.eat_fruit_data.e)->points_);
		break;

	case _m_EAT_GHOST:
		incrScore(mngr_->getComponent<Points>(m.eat_fruit_data.e)->points_);
		break;

	default:
		break;
	}
}