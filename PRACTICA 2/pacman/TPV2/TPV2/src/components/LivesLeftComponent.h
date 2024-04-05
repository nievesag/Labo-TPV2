#pragma once

#include "../ecs/Component.h"
#include <cassert>

class Texture;

class LivesLeftComponent : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::cmp::LIFES)

	LivesLeftComponent();
	LivesLeftComponent(int livesLeft);
	virtual ~LivesLeftComponent();

	void initComponent() override;
	void render() override;

	void setLives(int lives)
	{
		livesLeft_ = lives;
		assert(livesLeft_ >= 0);
	}

	int updateLives(int i) {

		// si quieres restar vidas pasar -1
		livesLeft_ += i;
		assert(livesLeft_ >= 0);
		return livesLeft_;
	}

	int getLives()
	{
		return livesLeft_;
	}

	void resetLives()
	{
		livesLeft_ = maxLives_;
	}

	Texture* getLifesTexture()
	{
		return img_;
	}

	int livesLeft_;
	int maxLives_ = 3;

private:
	Texture* img_;
};