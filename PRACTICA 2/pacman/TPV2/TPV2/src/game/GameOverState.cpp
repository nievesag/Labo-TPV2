#include "GameOverState.h"
#include "../ecs/Manager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "Game.h"

GameOverState::GameOverState()
{

}

GameOverState::~GameOverState()
{
}

void GameOverState::enter()
{
	if(true) // condicion de derrota
	{
		// guarda la textura correspondiente al menu de derrota
		msg_ = &sdlutils().msgs().at("game_over_lose");
	}
	else
	{
		// guarda la textura correspondiente al menu de victoria
		msg_ = &sdlutils().msgs().at("game_over_win");
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
	}

	// muestra mensaje
	sdlutils().clearRenderer();
	msg_->render(dest_);
	sdlutils().presentRenderer();
}

void GameOverState::leave()
{
}