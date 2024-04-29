// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <SDL_stdinc.h>
#include "SDLNetUtils.h"

#pragma pack(push,1)

enum MsgType : Uint8 {
	_NONE_ = 0, //
	_CONNECTION_ACCEPTED_, //
	_CONNECTION_REJECTED_, //
	_DISCONNECTED_, //
	_STATE_INFO_, //
	_BROADCAST_STATE_
};

struct Msg {
	Uint8 _type;

	_IMPL_SERIALIAZION_(_type)
};

struct MsgWithId: Msg {
	Uint8 _id;
	_IMPL_SERIALIAZION_WITH_BASE_(Msg,_id)

};

struct StateInfo: MsgWithId {
	Uint32 _state;
	_IMPL_SERIALIAZION_WITH_BASE_(MsgWithId,_state)
};


#pragma pack(pop)

