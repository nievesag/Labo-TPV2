// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <vector>
#include "../ecs/ecs.h"
#include "GameState.h"
#include "../utils/Singleton.h"

class InputHandler;

class Game : public Singleton<Game> {
public:
	// es singleton
	friend Singleton<Game>;

	// estados de juego
	enum State { RUNNING, PAUSED, NEWGAME, NEWROUND, GAMEOVER };

	// constructora y destructora
	Game();

	// libera la memoria de los estados
	virtual ~Game();

	// crea los sistemas y los estados
	void init();

	// El bucle principal tiene que incluir solo una llamada al update del
	// estado actual y a flush del Manager para enviar mensajes pendientes
	// (si usas el mecanismo de enviar mensajes con delay)
	void start();

	inline ecs::Manager* getMngr() { return mngr_; }

	inline void setState(State s) {

		GameState* new_state = nullptr;
		switch (s) {
		case RUNNING:
			new_state = runningState_;
			break;

		case PAUSED:
			new_state = pauseState_;
			break;

		case NEWGAME:
			new_state = newGameState_;
			break;

		case NEWROUND:
			new_state = newRoundState_;
			break;

		case GAMEOVER:
			new_state = gameOverState_;
			break;

		default:
			break;
		}
		currentState_->leave();
		currentState_ = new_state;
		currentState_->enter();
	}

private:
	// manager
	ecs::Manager *mngr_;

	// input
	InputHandler& ihdlr;

	// systems
	ecs::System *pacmanSys_;
	ecs::System	*ghostSys_;
	ecs::System *foodSys_;
	ecs::System *inmunitySys_;
	ecs::System *collisionSys_;
	ecs::System *renderSys_;
	ecs::System* gamectrlSys_;

	// estados
	GameState* runningState_;
	GameState* pauseState_;
	GameState* newGameState_;
	GameState* newRoundState_;
	GameState* gameOverState_;
	GameState* currentState_;
};