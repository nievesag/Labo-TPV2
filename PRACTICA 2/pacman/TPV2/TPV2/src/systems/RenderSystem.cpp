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

RenderSystem::RenderSystem() {
	

}

RenderSystem::~RenderSystem() {
}

void RenderSystem::initSystem() {
}

void RenderSystem::update() {
	drawMsgs();
	drawStars();
	drawPacMan();
	drawGhosts();
	drawLives();
	drawFruits();
}

void RenderSystem::drawStars() {
	// draw stars
	for (auto e : mngr_->getEntities(ecs::grp::STARS)) {

		auto tr = mngr_->getComponent<Transform>(e);
		auto tex = mngr_->getComponent<Image>(e)->tex_;
		draw(tr, tex);
	}
}

void RenderSystem::drawPacMan() {
	auto e = mngr_->getHandler(ecs::hdlr::PACMAN);
	/*auto tr = mngr_->getComponent<Transform>(e);
	auto tex = mngr_->getComponent<Image>(e)->tex_;
	draw(tr, tex);*/

	mngr_->getComponent<ImageWithFrames>(e)->render();
}

void RenderSystem::drawMsgs() {
	// draw the score
	//
	auto score = 0; // mngr_->getSystem<GameCtrlSystem>()->getScore();

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

void RenderSystem::drawGhosts()
{
	// draw stars
	for (auto e : mngr_->getEntities(ecs::grp::GHOSTS)) {

		/*auto tr = mngr_->getComponent<Transform>(e);
		auto tex = mngr_->getComponent<ImageWithFrames>(e)->tex_;
		draw(tr, tex);*/

		mngr_->getComponent<ImageWithFrames>(e)->render();
	}
}

void RenderSystem::drawLives()
{
	auto e = mngr_->getHandler(ecs::hdlr::PACMAN);
	mngr_->getComponent<LivesLeftComponent>(e)->render();
}

void RenderSystem::drawFruits()
{
	// draw stars
	for (auto e : mngr_->getEntities(ecs::grp::FRUITS)) {

		auto tr = mngr_->getComponent<Transform>(e);
		auto tex = mngr_->getComponent<Image>(e)->tex_;
		draw(tr, tex);
	}
}

void RenderSystem::draw(Transform *tr, Texture *tex) {
	SDL_Rect dest = build_sdlrect(tr->pos_, tr->width_, tr->height_);

	assert(tex != nullptr);
	tex->render(dest, tr->rot_);
}
