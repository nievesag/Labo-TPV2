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

		// systems
		pacmanSys_(), 
		renderSys_(), 
		collisionSys_(),
		ghostSys_(),
		foodSys_(),
		inmunitySys_(),
		gamectrlSys_(),

		// game states
		pauseState_(nullptr), 
		runningState_(nullptr), 
		newGameState_(nullptr), 
		newRoundState_(nullptr), 
		gameOverState_(nullptr)
{ }

Game::~Game() {
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

	// crea estados
	pauseState_ = new PauseState();
	runningState_ = new RunningState(); 
	newGameState_ = new NewGameState();
	newRoundState_ = new NewRoundState();
	gameOverState_ = new GameOverState();

	// inicializa en nuevo juego
	currentState_ = newGameState_;
}

void Game::start() {

	// a boolean to exit the loop
	bool exit = false;

	auto &ihdlr = ih();

	while (!exit) {
		Uint32 startTime = sdlutils().currRealTime();

		// refresh the input handler
		ihdlr.refresh();

		// si escape sales del juego
		if (ihdlr.isKeyDown(SDL_SCANCODE_ESCAPE)) {
			exit = true;
			continue;
		}

		// llama al update de los sistemas
		pacmanSys_->update();
		collisionSys_->update();
		ghostSys_->update();
		foodSys_->update();
		inmunitySys_->update();

		// refresh del manager (elimina entidades)
		mngr_->refresh();

		// limpia pantalla
		sdlutils().clearRenderer();

		// update del render
		renderSys_->update();

		// muestra render
		sdlutils().presentRenderer();

		// llama al update del estado actual
		currentState_->update();

		// frames del juego (tiempo)
		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 10)
			SDL_Delay(10 - frameTime);
	}
}