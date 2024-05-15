#include "GameOverState.h"
#include "../ecs/Manager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "Game.h"
#include "../components/LivesLeftComponent.h"

GameOverState::GameOverState() {}

GameOverState::~GameOverState() {}

void GameOverState::enter()
{
	// pausa el tiempo al entrar al estado
	sdlutils().virtualTimer().pause();

	auto pacman = mngr_->getHandler(ecs::hdlr::PACMAN);

	// si aun quedan vidas al entrar en el estado
	if(mngr_->getComponent<LivesLeftComponent>(pacman)->livesLeft_ > 0)
	{
		// guarda la textura correspondiente al menu de victoria
		msg_ = &sdlutils().msgs().at("game_over_win");

		sdlutils().soundEffects().at("won").play();
		sdlutils().soundEffects().at("won").setChannelVolume(20);

	}
	else
	{
		// guarda la textura correspondiente al menu de derrota
		msg_ = &sdlutils().msgs().at("game_over_lose");
	}

	// hace el rect destino
	dest_ = build_sdlrect((sdlutils().width() - msg_->width()) / 2, (sdlutils().height() - msg_->height()) / 2, // pos x, y
						  msg_->width(), msg_->height()); // tamanio w, h
}

void GameOverState::update()
{
	// si se pulsa enter pasas al estado newgame
	if (ih().keyDownEvent() && ih().isKeyDown(SDL_SCANCODE_RETURN)) {

		Game::instance()->setState(Game::NEWGAME);

		Message newGameMessage;
		newGameMessage.id = _m_NEW_GAME;
		Game::instance()->getMngr()->send(newGameMessage, true);
	}

	// muestra mensaje
	msg_->render(dest_);
}

void GameOverState::leave()
{
	sdlutils().soundEffects().at("won").pauseChannel();


	// reanuda el tiempo al salir del estado
	sdlutils().virtualTimer().resume();
}