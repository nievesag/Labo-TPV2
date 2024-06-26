#pragma once

#include "../ecs/Component.h"
#include <cassert>

class Texture;

struct LivesLeftComponent : public ecs::Component
{
	__CMPID_DECL__(ecs::cmp::LIFES)

	LivesLeftComponent();
	LivesLeftComponent(int livesLeft);
	virtual ~LivesLeftComponent();

	void initComponent() override;
	void render() override;

	int updateLives(int i)
	{
		// si quieres restar vidas pasar -1
		livesLeft_ += i;
		assert(livesLeft_ >= 0);
		return livesLeft_;
	}

	void resetLives()
	{
		livesLeft_ = maxLives_;
	}

	int livesLeft_;
	int maxLives_ = 3;

private:
	Texture* img_;
};