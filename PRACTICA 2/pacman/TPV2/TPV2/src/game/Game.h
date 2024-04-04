// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <vector>
#include "../ecs/ecs.h"
#include "GameState.h"
#include "../utils/Singleton.h"

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

	// El bucle principal tiene que incluir solo una llamada al update del
	// estado actual y a flush del Manager para enviar mensajes pendientes
	// (si usas el mecanismo de enviar mensajes con delay)

	// crea los sistemas y los estados
	void init();
	void start();

private:
	// manager
	ecs::Manager *mngr_;

	// systems
	ecs::System *pacmanSys_;
	ecs::System	*ghostSys_;
	ecs::System *foodSys_;
	ecs::System *inmunitySys_;
	ecs::System *collisionSys_;
	ecs::System *renderSys_;

	// estados
	GameState* runningState_;
	GameState* pauseState_;
	GameState* newGameState_;
	GameState* newRoundState_;
	GameState* gameOverState_;
	//GameState* current_state_; // de la anterior practica no se si hace falta aqui
};