// This file is part of the course TPV2@UCM - Samir Genaim

#include "Game.h"

// util
#include "../ecs/Manager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"

// componentes basicos
#include "../utils/Vector2D.h"
#include "../utils/Collisions.h"

// systems
#include "../systems/CollisionsSystem.h"
#include "../systems/PacManSystem.h"
#include "../systems/FoodSystem.h"
#include "../systems/GhostSystem.h"
#include "../systems/InmunitySystem.h"
#include "../systems/RenderSystem.h"

// estados
#include "GameOverState.h"
#include "NewGameState.h"
#include "NewRoundState.h"
#include "PauseState.h"
#include "RunningState.h"

using ecs::Manager;

Game::Game() :
		// manager
		mngr_(),

		// input handler
		ihdlr(ih()),

		// game states
		pauseState_(nullptr), 
		runningState_(nullptr), 
		newGameState_(nullptr), 
		newRoundState_(nullptr), 
		gameOverState_(nullptr)
{ }

Game::~Game() {
	// libera la memoria de los estados
	delete pauseState_;
	delete runningState_;
	delete newGameState_;
	delete newRoundState_;
	delete gameOverState_;

	// borra el manager
	delete mngr_;
}

void Game::init() {

	// initialise the SDLUtils singleton
	SDLUtils::init("Demo", 800, 600, "resources/config/resources.json");

	// Create the manager
	mngr_ = new Manager();

	// crea systems
	pacmanSys_ = mngr_->addSystem<PacManSystem>();
	renderSys_ = mngr_->addSystem<RenderSystem>();
	collisionSys_ = mngr_->addSystem<CollisionsSystem>();
	ghostSys_ = mngr_->addSystem<GhostSystem>();
	foodSys_ = mngr_->addSystem<FoodSystem>();
	inmunitySys_ = mngr_->addSystem<InmunitySystem>();
	gamectrlSys_ = mngr_->addSystem<GameCtrlSystem>();

	// crea estados
	pauseState_ = new PauseState();
	runningState_ = new RunningState(); 
	newGameState_ = new NewGameState();
	newRoundState_ = new NewRoundState();
	gameOverState_ = new GameOverState();

	// para poder usar mngr_ desde fuera
	pauseState_->setContext(mngr_);
	runningState_->setContext(mngr_);
	newGameState_->setContext(mngr_);
	newRoundState_->setContext(mngr_);
	gameOverState_->setContext(mngr_);

	// inicializa en nuevo juego
	currentState_ = newGameState_;
	runningState_->enter();
}

void Game::start() {

	// a boolean to exit the loop
	bool exit = false;

	auto &ihdlr = ih();

	while (!exit) {
		Uint32 startTime = sdlutils().currRealTime();

		// limpia pantalla
		sdlutils().clearRenderer();

		// refresh the input handler
		ihdlr.refresh();

		// si escape sales del juego
		if (ihdlr.isKeyDown(SDL_SCANCODE_ESCAPE)) {
			exit = true;
			continue;
		}

		// llama al update del estado actual
		currentState_->update();

		// update del render
		renderSys_->update();

		// muestra render
		sdlutils().presentRenderer();

		// flush del manager para enviar mensajes pendientes
		// (si usas el mecanismo de enviar mensajes con delay)
		mngr_->flushMessages();

		// refresh del manager (elimina entidades)
		mngr_->refresh();

		// frames del juego (tiempo)
		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 10)
			SDL_Delay(10 - frameTime);
	}
}