// This file is part of the course TPV2@UCM - Samir Genaim

#include "Game.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "LittleWolf.h"
#include "Networking.h"


Game::Game() :
		little_wolf_(nullptr), //
		networking_(nullptr)
{
}

Game::~Game()
{
	delete little_wolf_;
	delete networking_;
}

bool Game::init(const char* host, Uint16 port)
{
	networking_ = new Networking();

	if (!networking_->init(host, port)) 
	{
		SDLNetUtils::print_SDLNet_error();
	}

	std::cout << "Connected as client " << (int)networking_->client_id() << std::endl;

	std::cout << "Little Wolf de este cliente inicializando..." << std::endl;

	// initialize the SDLUtils singleton
	SDLUtils::init("Demo", 900, 480, "resources/config/littlewolf.resources.json");

	// cada cliente crea su juego
	little_wolf_ = new LittleWolf(sdlutils().width(), sdlutils().height(), sdlutils().window(), sdlutils().renderer());

	// load a map
	little_wolf_->load("resources/maps/little_wolf/map_0.txt");


	// add player
	little_wolf_->addPlayer(networking_->client_id());
	little_wolf_->send_my_info();


	return true;
}

void Game::start() {

	// a boolean to exit the loop
	bool exit = false;

	auto &ihdlr = ih();

	std::cout << "Game Start!!" << std::endl;

	while (!exit) 
	{
		Uint32 startTime = sdlutils().currRealTime();

		// refresh the input handler
		ihdlr.refresh();

		if (ihdlr.keyDownEvent()) 
		{
			// ESC exists the game
			if (ihdlr.isKeyDown(SDL_SCANCODE_ESCAPE)) 
			{
				exit = true;
				continue;
			}

			// N switches to the next player view
			if (ihdlr.isKeyDown(SDL_SCANCODE_N)) 
			{
				little_wolf_->switchToNextPlayer();
			}

			// R brings deads to life
			if (ihdlr.isKeyDown(SDL_SCANCODE_R)) 
			{
				little_wolf_->bringAllToLife();
			}
		}

		// update
		little_wolf_->update();
		little_wolf_->send_my_info();
		networking_->update();


		
		// the clear is not necessary since we copy the whole texture -- I guess ...
		// sdlutils().clearRenderer();

		// render
		little_wolf_->render();
		sdlutils().presentRenderer();

		// control de tiempo
		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 10)
		{
			SDL_Delay(10 - frameTime);
		}
	}

	networking_->disconnect();
}

LittleWolf* Game::get_wolves()
{
	return little_wolf_;
}

Networking* Game::get_networking()
{
	return networking_;
}