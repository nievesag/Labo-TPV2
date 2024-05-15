// This file is part of the course TPV2@UCM - Samir Genaim

#include "RenderSystem.h"

#include "../components/Image.h"
#include "../components/ImageWithFrames.h"
#include "../components/Transform.h"
#include "../ecs/Manager.h"
#include "../sdlutils/macros.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/Texture.h"
#include "GameCtrlSystem.h"
#include "../components/LivesLeftComponent.h"

RenderSystem::RenderSystem() {}

RenderSystem::~RenderSystem() {}

void RenderSystem::initSystem() {}

void RenderSystem::update()
{
	if(renderGame)
	{
		// en cada frame dibuja lo que debe
		drawPacMan();
		drawGhosts();
		drawFruits();
		drawLives();
		drawMsgs();
	}
}

void RenderSystem::recieve(const Message& m)
{
	switch(m.id)
	{
	case _m_ROUND_START:
		renderGame = true;
		break;

	case _m_GAME_OVER:
		renderGame = false;
		break;

	case _m_WIN_GAME:
		renderGame = false;
		break;

	case _m_ROUND_OVER:
		renderGame = false;
		break;

	default:
		break;
	}
}

void RenderSystem::drawPacMan()
{
	// pinta a pacman
	auto e = mngr_->getHandler(ecs::hdlr::PACMAN);
	mngr_->getComponent<ImageWithFrames>(e)->render();
}

void RenderSystem::drawGhosts()
{
	// pinta fantasmas
	for (auto e : mngr_->getEntities(ecs::grp::GHOSTS)) 
	{
		mngr_->getComponent<ImageWithFrames>(e)->render();
	}
}

void RenderSystem::drawLives()
{
	// pinta la interfaz de vidas
	auto e = mngr_->getHandler(ecs::hdlr::PACMAN);
	mngr_->getComponent<LivesLeftComponent>(e)->render();
}

void RenderSystem::drawFruits()
{
	// dibuja las frutas
	for (auto e : mngr_->getEntities(ecs::grp::FRUITS)) 
	{
		mngr_->getComponent<ImageWithFrames>(e)->render();
	}
}

void RenderSystem::drawMsgs()
{
	// draw the score
	auto score = mngr_->getSystem<GameCtrlSystem>()->getScore();

	Texture scoreTex(sdlutils().renderer(), std::to_string(score),
		sdlutils().fonts().at("ARIAL24"), build_sdlcolor(0x444444ff));

	SDL_Rect dest = build_sdlrect( //
		(sdlutils().width() - scoreTex.width()) / 2.0f, //
		10.0f, //
		scoreTex.width(), //
		scoreTex.height());

	scoreTex.render(dest);

	// draw add stars message
	sdlutils().msgs().at("addstars").render(10, 10);
}

/*
// no se usa
void RenderSystem::draw(Transform *tr, Texture *tex)
{
	SDL_Rect dest = build_sdlrect(tr->pos_, tr->width_, tr->height_);
	assert(tex != nullptr);
	tex->render(dest, tr->rot_);
}
*/