#pragma once

#include "../ecs/System.h"

#include <algorithm>
#include "../components/Image.h"
#include "../components/Points.h"
#include "../components/StarMotion.h"
#include "../components/Transform.h"
#include "../ecs/Manager.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"
#include "GameCtrlSystem.h"

class FoodSystem : public ecs::System
{
public:
	__SYSID_DECL__(ecs::sys::FOOD)

	FoodSystem();
	virtual ~FoodSystem();
	void initSystem() override;
	void update() override;
	void recieve(const Message& m) override;

	void setFruits();
	void updateState();
	void destroyFruit(ecs::entity_t fruit);
	bool noFruits();
	void end();

private:
	int totalFruits;
	int maxFruits = 20;

	int cols, fils;

	int offsetX = 150;
	int offsetY = 150;

};