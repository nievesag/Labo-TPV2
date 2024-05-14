#pragma once

#include "../ecs/System.h"

#include <algorithm>
#include "../components/Image.h"
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
	void destroyFruit(ecs::entity_t fruit);
	bool noFruits();
	void resetFruits();
	void miraculousFruitManager();
	void foodCollision(ecs::entity_t fruit);

private:
	int totalFruits;
	int maxFruits = 20;

	int cols, fils;

	int offsetX = 150;
	int offsetY = 150;

	// cuando generamos frutas, con probabilidad del 0.1 se genera una fruta milagrosa
	int isMiraculousChance = 100;
};