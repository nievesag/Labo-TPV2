#pragma once

#include <SDL_net.h>
#include <SDL_stdinc.h>


// HACER CLASE PARA LOS TIPOS DE MENSAJES

class Multiplayer {
public:

	Multiplayer();
	virtual ~Multiplayer();

	bool init(const char* host, Uint16 port);
	bool disconnect();
	void update();


private:


};
