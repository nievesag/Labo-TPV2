// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <SDL_stdinc.h>

#pragma pack(push,1)

enum MsgType : Uint8 {
	_NONE_ = 0, //
	_FIND_SERVER_, //
	_I_AM_THE_SERVER_, //
	_CONNECTION_REQUEST_, //
	_CONNECTION_ACCEPTED_, //
	_CONNECTION_REJECTED_, //
	_DISCONNECTED_, //
	_SHUTDOWN_, //
	_STATE_INFO_, //
	_BROADCAST_STATE_
};

struct NetworkMessage {
	Uint8 _type;
	Uint8 _id;
	Uint32 _state;
};

#pragma pack(pop)

