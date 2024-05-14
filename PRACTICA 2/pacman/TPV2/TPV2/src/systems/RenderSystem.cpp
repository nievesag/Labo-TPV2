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
	// en cada frame dibuja lo que debe
	drawPacMan();
	drawGhosts();
	drawLives();
	drawFruits();
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

/*
// no se usa
void RenderSystem::draw(Transform *tr, Texture *tex)
{
	SDL_Rect dest = build_sdlrect(tr->pos_, tr->width_, tr->height_);
	assert(tex != nullptr);
	tex->render(dest, tr->rot_);
}
*/