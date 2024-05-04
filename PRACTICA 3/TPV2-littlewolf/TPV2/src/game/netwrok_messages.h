// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <SDL_stdinc.h>
#include "../sdlutils/SDLNetUtils.h"
#include "LittleWolf.h"


class LittleWolf;

enum MsgType : Uint8 {
	_NONE = 0, //
	_CONNECTION_REQUEST, //
	_CONNECTION_ACCEPTED, //
	_CONNECTION_REJECTED, //
	_DISCONNECTED,
	_NEW_CLIENT_CONNECTED, //
	_PLAYER_STATE, //
	_PLAYER_INFO, //
	_SHOOT, //
	_DEAD, //
	_RESTART
};

struct Msg {
	Uint8 _type;

	_IMPL_SERIALIAZION_(_type)
};

struct MsgWithId: Msg {
	Uint8 _client_id;

	_IMPL_SERIALIAZION_WITH_BASE_(Msg, _client_id)
};

struct MsgWithMasterId: MsgWithId {
	Uint8 _master_id;

	_IMPL_SERIALIAZION_WITH_BASE_(MsgWithId, _master_id)
};

struct PlayerStateMsg: MsgWithId {

	float x;
	float y;
	int w;
	int h;
	float rot;

	_IMPL_SERIALIAZION_WITH_BASE_(MsgWithId, x, y, w, h, rot)
};

struct PlayerInfoMsg: MsgWithId {

	float posX;   // X pos
	float posY;   // Y pos
	float velX;   // X vel     
	float velY;   // Y vel
	float speed;  // speed
	float acc;    // acceleration
	float theta;  // rotation (in rad)
	Uint8 state;  // state

	_IMPL_SERIALIAZION_WITH_BASE_(MsgWithId, posX, posY, velX, velY, speed, acc, theta, state)
};

struct ShootMsg: MsgWithId {

	float x;
	float y;
	float vx;
	float vy;
	int w;
	int h;
	float rot;

	_IMPL_SERIALIAZION_WITH_BASE_(MsgWithId, x,y,vx,vy,w,h,rot)
};