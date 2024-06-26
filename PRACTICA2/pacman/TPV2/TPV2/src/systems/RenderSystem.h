// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/System.h"

class Texture;
struct Transform;

class RenderSystem : public ecs::System
{
public:
	__SYSID_DECL__(ecs::sys::RENDER)

	RenderSystem();
	virtual ~RenderSystem();
	void initSystem() override;
	void update() override;
	void recieve(const Message& m) override;

private:
	bool renderGame = false;

	void drawPacMan();
	void drawGhosts();
	void drawLives();
	void drawFruits();
	void drawMsgs();
};