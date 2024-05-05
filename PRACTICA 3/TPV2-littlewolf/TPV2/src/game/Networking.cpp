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
	clientId_(0), //
	masterId_() {
}

Networking::~Networking() {}

// INIT
bool Networking::init(const char *host, Uint16 port) {

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
			handle_disconnect(m1._client_id); // se administra la desconexion
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
	

		case _WAITING_SCREEN:
			handle_waiting();
			break;

		default:
			break;
		}
	}
}

#pragma region SEND COSAS
void Networking::send_my_info(const Vector2D& pos, const Vector2D& vel, float speed, float acceleration, float theta, Uint8 state) 
{
	// mensaje
	PlayerInfoMsg m;

	// id del jugador
	m._client_id = clientId_;

	// mensaje de info de jugador
	m._type = _PLAYER_INFO;

	// info del jugador
	m.posX = pos.getX();
	m.posY = pos.getY();
	m.velX = vel.getX();
	m.velY = vel.getY();
	m.speed = speed;
	m.theta = theta;
	m.state = state;

	std::cout << "current state of player " << clientId_ << " is " << (int)state << std::endl;

	/*std::cout << p_ << std::endl;
	std::cout << sock_ << std::endl;
	std::cout << srvadd_.host << std::endl;*/

	// lo envia de manera serializada
	SDLNetUtils::serializedSend(m, p_, sock_, srvadd_);
}

void Networking::send_shoot_request()
{
	// mensaje
	MsgWithId m;

	// mensaje de muerte
	m._type = _SHOOT_REQUEST;

	// lo envia de manera serializada
	SDLNetUtils::serializedSend(m, p_, sock_, srvadd_);
}

void Networking::send_move_request()
{
	// mensaje
	MsgWithId m;

	// mensaje de muerte
	m._type = _MOVE_REQUEST;

	// lo envia de manera serializada
	SDLNetUtils::serializedSend(m, p_, sock_, srvadd_);
}

void Networking::send_shoot(Vector2D p, Vector2D v, int width, int height, float r)
{
	// mensaje
	ShootMsg m;

	// mensaje de info del disparo
	m._type = _SHOOT;

	// id del jugador que dispara
	m._client_id = clientId_;

	// info del disparo
	m.x = p.getX();
	m.y = p.getY();
	m.vx = v.getX();
	m.vy = v.getY();
	m.w = width;
	m.h = height;
	m.rot = r;

	// lo envia de manera serializada
	SDLNetUtils::serializedSend(m, p_, sock_, srvadd_);
}

void Networking::send_dead(Uint8 id) {

	// mensaje
	MsgWithId m;

	// mensaje de muerte
	m._type = _DEAD;

	// id del jugador que muere
	m._client_id = id;

	// lo envia de manera serializada
	SDLNetUtils::serializedSend(m, p_, sock_, srvadd_);
}

void Networking::send_waiting()
{
	Msg m;

	// mensaje de muerte
	m._type = _WAITING_SCREEN;

	// lo envia de manera serializada
	SDLNetUtils::serializedSend(m, p_, sock_, srvadd_);
}

void Networking::send_new_start()
{
	Msg m;

	// mensaje de muerte
	m._type = _NEW_START;

	// lo envia de manera serializada
	SDLNetUtils::serializedSend(m, p_, sock_, srvadd_);
}

void Networking::send_restart() {

	// mensaje
	Msg m;

	// mensaje de restart
	m._type = _RESTART;

	// lo envia de manera serializada
	SDLNetUtils::serializedSend(m, p_, sock_, srvadd_);
}
#pragma endregion

#pragma region HANDLE COSAS
void Networking::handle_new_client(Uint8 id) {

	// si se mete alguien que no estaba
	if (id != clientId_) 
	{
		// llama al metodo de little wolf que envia info de jugador
		Game::instance()->get_wolves()->send_my_info();
	}
}

void Networking::handle_disconnect(Uint8 id) {

	std::cout << "disconnecting..." << std::endl;

	// llama al metodo de little wolf que elimina jugador
	Game::instance()->get_wolves()->removePlayer(id);
}

void Networking::handle_dead(const MsgWithId &m) {

	// llama al metodo de little wolf que mata jugador
	Game::instance()->get_wolves()->killPlayer();
}

void Networking::handle_waiting()
{
	// llama al metodo de little wolf que mata jugador
	Game::instance()->get_wolves()->process_waiting();
}

void Networking::handle_shoot_request()
{
	// llama al metodo de little wolf que mata jugador
	//Game::instance()->get_wolves()->process_shoot_request();
}

void Networking::handle_move_request()
{
	//Game::instance()->get_wolves()->process_move_request();
}

void Networking::handle_new_start()
{
	Game::instance()->get_wolves()->process_new_start();
}

void Networking::handle_player_info(const PlayerInfoMsg &m) {

	if (m._client_id != clientId_) 
	{
		Game::instance()->get_wolves()->update_player_info(m._client_id, m.posX, m.posY, m.velX, m.velY, m.speed, m.acc, m.theta, (LittleWolf::PlayerState)m.state);
	}
}

void Networking::handle_shoot(const ShootMsg& m)
{
	//Game::instance()->get_wolves().get_bullets().shoot(Vector2D(m.x, m.y), Vector2D(m.vx, m.vy), m.w, m.h, m.rot);
}

void Networking::handle_restart() {
	Game::instance()->get_wolves()->bringAllToLife();
}
void Networking::handle_player_state()
{


}
#pragma endregion