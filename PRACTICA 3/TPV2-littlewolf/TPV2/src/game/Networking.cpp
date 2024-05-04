// This file is part of the course TPV2@UCM - Samir Genaim

#include "Networking.h"

#include <iostream>
#include "../utils/Vector2D.h"

#include "LittleWolf.h"
#include "Game.h"
#include "netwrok_messages.h"
#include "../sdlutils/SDLNetUtils.h"

Networking::Networking() :
		sock_(), //
		socketSet_(), //
		p_(), //
		srvadd_(), //
		clientId_(), //
		masterId_() {
}

Networking::~Networking() {
}

bool Networking::init(char *host, Uint16 port) {

	if (SDLNet_ResolveHost(&srvadd_, host, port) < 0) {
		SDLNetUtils::print_SDLNet_error();
		return false;
	}

	// los sockets son un mecanismo que permiten la comunicacion entre programas en el mismo pc o distintos
	// comunicacion bidireccional en una misma maquina o entre procesos lanzados en diferentes maquinas
	sock_ = SDLNet_UDP_Open(0); 

	if (!sock_) {
		SDLNetUtils::print_SDLNet_error();
		return false;
	}

	p_ = SDLNet_AllocPacket(512);

	if (!p_) {
		SDLNetUtils::print_SDLNet_error();
		return false;
	}

	socketSet_ = SDLNet_AllocSocketSet(1);
	SDLNet_UDP_AddSocket(socketSet_, sock_);

	Msg m0;
	MsgWithMasterId m1;

	// request to connect
	m0._type = _CONNECTION_REQUEST;
	SDLNetUtils::serializedSend(m0, p_, sock_, srvadd_);

	bool connected = false;

	// wait 3sec for the response, if we don't get we stop (since done will be 'true')
	if (SDLNet_CheckSockets(socketSet_, 3000) > 0) 
	{
		// si la data de socket esta preparada para ser leida
		if (SDLNet_SocketReady(sock_)) 
		{

			if (SDLNetUtils::deserializedReceive(m0, p_, sock_) > 0) 
			{
				// actua segun la respuesta de la request de conexion
				switch (m0._type)
				{
				// si se acepta la conexion
				case _CONNECTION_ACCEPTED:
					m1.deserialize(p_->data);
					clientId_ = m1._client_id;
					masterId_ = m1._master_id;
					connected = true;
					break;

				// si se rechaza la conexion
				case _CONNECTION_REJECTED:
					break;
				}
			}
		}
	}

	// si despues de procesar el mensaje no llega a conectarse
	if (!connected) 
	{
		std::cerr << "Failed to get a client id" << std::endl;
		return false;
	}

	#ifdef _DEBUG
	std::cout << "Connected with id " << (int) clientId_ << std::endl;
	#endif

	return true; // inicializacion completada
}

bool Networking::disconnect() {
	MsgWithId m;
	m._type = _DISCONNECTED;
	m._client_id = clientId_;
	return (SDLNetUtils::serializedSend(m, p_, sock_, srvadd_) > 0);
}

// AQUI SE ADMINISTRAN TODOS LOS MENSAJES
void Networking::update() {

	Msg m0; // mensajes de flujo
	MsgWithMasterId m1; // para mandarle info al master
	PlayerStateMsg m2; // para informar al master de estados de jugadores
	ShootMsg m3; // mensaje de disparo
	MsgWithId m4; // para muertes
	PlayerInfoMsg m5; // para informar al master de informacion de jugadores

	while (SDLNetUtils::deserializedReceive(m0, p_, sock_) > 0) {

		// actua segun los mensajes que se van recibiendo
		switch (m0._type) {
		case _NEW_CLIENT_CONNECTED:
			// informas al master de nuevo jugador conectado
			m1.deserialize(p_->data); // al enviar un mensaje hay que deserializarlo para poder usarlo
			masterId_ = m1._master_id; // guardas el nuevo master si hay que cambiarlo
			handle_new_client(m1._client_id); // se administra nuevo jugador
			break;

		case _DISCONNECTED:
			// informas al master de jugador desconectado
			m1.deserialize(p_->data); 
			masterId_ = m1._master_id; // guardas el nuevo master si hay que cambiarlo
			handle_disconnet(m1._client_id); // se administra la desconexion
			break;

		case _PLAYER_STATE:
			// para informar al master del estado de los jugadores
			m2.deserialize(p_->data);
			handle_player_state(m2);
			break;

		case _PLAYER_INFO:
			// para informar al master de info de los jugadores
			m5.deserialize(p_->data);
			handle_player_info(m5);
			break;

		case _SHOOT:
			// para informar al master de disparos
			m3.deserialize(p_->data);
			handle_shoot(m3);
			break;

		case _DEAD:
			// para informar al master de muertes
			m4.deserialize(p_->data);
			handle_dead(m4);
			break;

		case _RESTART:
			// para resetear
			handle_restart();
			break;

		default:
			break;
		}
	}
}

void Networking::handle_new_client(Uint8 id) {

	// si se mete alguien que no estaba
	if (id != clientId_) 
	{
		Game::instance()->get_wolves().send_my_info();
	}
}

void Networking::handle_disconnet(Uint8 id) {

	Game::instance()->get_wolves().removePlayer(id);
}

void Networking::send_state(const Vector2D& pos, float w, float h, float rot) {

	// mensaje
	PlayerStateMsg m;

	// tipo de mensaje: estado de jugador
	m._type = _PLAYER_STATE;

	// id del cliente
	m._client_id = clientId_;

	// info de estado
	m.x = pos.getX();
	m.y = pos.getY();
	m.w = w;
	m.h = h;
	m.rot = rot;

	SDLNetUtils::serializedSend(m, p_, sock_, srvadd_);
}

void Networking::handle_player_state(const PlayerStateMsg &m) {

	// AQUI FALTAN COSAS 
}

void Networking::send_shoot(Vector2D p, Vector2D v, int width, int height,
		float r) {
	ShootMsg m;
	m._type = _SHOOT;
	m._client_id = clientId_;
	m.x = p.getX();
	m.y = p.getY();
	m.vx = v.getX();
	m.vy = v.getY();
	m.w = width;
	m.h = height;
	m.rot = r;
	SDLNetUtils::serializedSend(m, p_, sock_, srvadd_);
}

void Networking::send_dead(Uint8 id) {
	MsgWithId m;
	m._type = _DEAD;
	m._client_id = id;
	SDLNetUtils::serializedSend(m, p_, sock_, srvadd_);
}

void Networking::handle_dead(const MsgWithId &m) {
	Game::instance()->get_wolves().killPlayer(m._client_id);
}

void Networking::send_my_info(const Vector2D& pos, const Vector2D& vel, float speed, float acceleration, float theta, Uint8 state) {
	// mensaje
	PlayerInfoMsg m;

	// mensaje de info de jugador
	m._type = _PLAYER_INFO;

	// id del jugador
	m._client_id = clientId_;

	// info del jugador
	m.posX = pos.getX();
	m.posY = pos.getY();
	m.velX = vel.getX();
	m.velY = vel.getY();
	m.speed = speed;
	m.theta = theta;
	m.state = state;

	// lo envia de manera serializada
	SDLNetUtils::serializedSend(m, p_, sock_, srvadd_);
}

void Networking::handle_player_info(const PlayerInfoMsg &m) {
	if (m._client_id != clientId_) 
	{
		Game::instance()->get_wolves().update_player_info(m._client_id, m.posX, m.posY, m.velX, m.velY, m.speed, m.acc, m.theta, (LittleWolf::PlayerState)m.state);
	}
}

void Networking::handle_shoot(const ShootMsg& m)
{
	//Game::instance()->get_wolves().get_bullets().shoot(Vector2D(m.x, m.y), Vector2D(m.vx, m.vy), m.w, m.h, m.rot);
}

void Networking::send_restart() {
	Msg m;
	m._type = _RESTART;
	SDLNetUtils::serializedSend(m, p_, sock_, srvadd_);
}

void Networking::handle_restart() {
	Game::instance()->get_wolves().bringAllToLife();
}