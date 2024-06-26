// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <SDL_net.h>
#include <SDL_stdinc.h>

#include "netwrok_messages.h"
class Vector2D;


class Networking {
public:
	Networking();
	virtual ~Networking();

	bool init(const char *host, Uint16 port);
	bool disconnect();
	void update();

	Uint8 client_id() {
		return clientId_;
	}

	bool is_master() {
		return clientId_ == masterId_;
	}

	// ---- SEND ----
	// manda los mensajes para que se procesen desde el propio networking
	void send_my_info(const Vector2D& pos, const Vector2D& vel, float speed, float acceleration, float theta, Uint8 state);

	void send_shoot_request();
	void send_move_request();
	void send_shoot(Vector2D p, Vector2D v, int width, int height, float r);
	void send_dead(Uint8 id);

	void send_waiting();
	void send_new_start();
	void send_restart();
	void send_synconize(Uint8 id, const Vector2D& pos);
	void send_sound();
	void send_pain();

private:
	// desde aqui se llaman a los metodos de cada cliente
	void handle_new_client(Uint8 id);
	void handle_disconnect(Uint8 id);
	void handle_player_info(const PlayerInfoMsg &m);
	void handle_dead(const MsgWithId &m);
	void handle_waiting();
	void handle_shoot_request(const ShootMsg& m);
	void handle_move_request(const MsgWithId& m);
	void handle_new_start();
	void handle_syncronize(PlayerInfoMsg& m);
	void handle_sound();
	void handle_pain();

	UDPsocket sock_;
	SDLNet_SocketSet socketSet_;
	UDPpacket *p_;
	IPaddress srvadd_;
	Uint8 clientId_;
	Uint8 masterId_;
};