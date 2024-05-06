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

	Msg m0; // mensaje normal
	MsgWithMasterId m_master; // mensaje con id del master
	ShootMsg m_shoot; // mensaje de disparo
	MsgWithId m_id; // mensaje con id del jugador
	PlayerInfoMsg m_info; // mensaje con informacion de jugadores

	while (SDLNetUtils::deserializedReceive(m0, p_, sock_) > 0) {

		// actua segun los mensajes que se van recibiendo
		switch (m0._type) {
		case _NEW_CLIENT_CONNECTED:
			// informas al master de nuevo jugador conectado
			m_master.deserialize(p_->data); // al enviar un mensaje hay que deserializarlo para poder usarlo
			masterId_ = m_master._master_id; // guardas el nuevo master si hay que cambiarlo
			handle_new_client(m_master._client_id); // se administra nuevo jugador
			break;

		case _DISCONNECTED:
			// informas al master de jugador desconectado
			m_master.deserialize(p_->data); 
			masterId_ = m_master._master_id; // guardas el nuevo master si hay que cambiarlo
			handle_disconnect(m_master ._client_id); // se administra la desconexion
			break;

		case _PLAYER_INFO:
			// para informar al master de info de los jugadores
			m_info.deserialize(p_->data);
			handle_player_info(m_info);
			break;

		case _DEAD:
			// para informar al master de muertes
			m_id.deserialize(p_->data);
			handle_dead(m_id);
			break;

		case _WAITING_SCREEN:
			handle_waiting();
			break;

		case _NEW_START:
			handle_new_start();
			break;

		case _SHOOT_REQUEST:
			// para pedir al master disparar
			m_shoot.deserialize(p_->data);
			handle_shoot_request(m_shoot);
			break;

		case _MOVE_REQUEST:
			// para pedir al master moverse
			m_id.deserialize(p_->data);
			handle_move_request(m_id);
			break;
		case _SYNCRONIZE:
			// updatea la info de los jugadores
			m_info.deserialize(p_->data);
			handle_syncronize(m_info);
			break;

		case _SOUND:
			// informas al master de jugador desconectado
			m_info.deserialize(p_->data);
			handle_sound();
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

	//std::cout << "current state of player " << (int)clientId_ << " is " << (int)state << std::endl;

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
	m._client_id = clientId_;

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

void Networking::send_dead(Uint8 id)
{
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

void Networking::send_restart()
{
	// mensaje
	Msg m;

	// mensaje de restart
	m._type = _RESTART;

	// lo envia de manera serializada
	SDLNetUtils::serializedSend(m, p_, sock_, srvadd_);
}
void Networking::send_synconize(Uint8 id, const Vector2D& pos)
{
	PlayerInfoMsg m;
	m._client_id = id;
	m.posX = pos.getX();
	m.posY = pos.getY();
	m._type = _SYNCRONIZE;

	SDLNetUtils::serializedSend(m, p_, sock_, srvadd_);
}
void Networking::send_sound()
{
	// mensaje
	Msg m;

	// mensaje de restart
	m._type = _SOUND;

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
	Game::instance()->get_wolves()->process_player_die(m._client_id);
}

void Networking::handle_waiting()
{
	// llama al metodo de little wolf que mata jugador
	Game::instance()->get_wolves()->process_waiting();
}

void Networking::handle_shoot_request(const ShootMsg& m)
{
	// SOLO EL MASTER PUEDE PROCESAR REQUESTS !!!!!!!!!!
	if (is_master()) 
	{
		std::cout << "BANG" << "\n";
		Game::instance()->get_wolves()->process_shoot_request(m._client_id);
	}
}

void Networking::handle_move_request(const MsgWithId& m)
{
	// SOLO EL MASTER PUEDE PROCESAR REQUESTS !!!!!!!!!!
	if (is_master())
	{
		Game::instance()->get_wolves()->process_shoot_request(m._client_id);
	}
}

void Networking::handle_new_start()
{
	if (is_master())
	{
		Game::instance()->get_wolves()->process_new_start();
	}
}

void Networking::handle_syncronize(PlayerInfoMsg& m)
{
	//std::cout << "PLAYER ID: " << (int)m._client_id << "      POSITION: " << m.posX << "," << m.posY << std::endl;
	Game::instance()->get_wolves()->player_syncronize(m._client_id, Vector2D(m.posX, m.posY));
}

void Networking::handle_sound()
{
	Game::instance()->get_wolves()->process_sound();
}

void Networking::handle_player_info(const PlayerInfoMsg &m)
{
	if (m._client_id != clientId_) 
	{
		Game::instance()->get_wolves()->update_player_info(m._client_id, m.posX, m.posY, m.velX, m.velY, m.speed, m.acc, m.theta, (LittleWolf::PlayerState)m.state);
	}
}
#pragma endregion