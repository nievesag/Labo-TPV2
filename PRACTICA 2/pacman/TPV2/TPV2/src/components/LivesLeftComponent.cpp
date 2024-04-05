#include "LivesLeftComponent.h"

#include <SDL_rect.h>
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/Texture.h"

LivesLeftComponent::LivesLeftComponent() :
	LivesLeftComponent(0) {}

LivesLeftComponent::LivesLeftComponent(int livesLeft) :
	livesLeft_(livesLeft), img_(&sdlutils().images().at("heart"))
{
	assert(livesLeft_ >= 0);
}

LivesLeftComponent::~LivesLeftComponent() {}

void LivesLeftComponent::initComponent() {}

void LivesLeftComponent::render()
{
	SDL_Rect dest{ 0, 5, 30, 30 };
	for (int i = 0; i < livesLeft_; i++) {
		dest.x = 2 + i * 30;
		img_->render(dest);
	}
}