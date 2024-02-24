#pragma once

#include "BlackHoleFacade.h"

class Vector2D;
class RandomNumberGenerator;

class BlackHoleUtils : public BlackHoleFacade {
public:
	BlackHoleUtils();
	virtual ~BlackHoleUtils();
	void remove_all_blackholes() override;
	void create_blackholes(int n) override;
	//void split_blackholes(ecs::entity_t a) override;

private:
	void generateBlackHole(const Vector2D& p, int r, int g);
	RandomNumberGenerator& rand_;
	int size_ = 30;
	ecs::Manager* mngr;

};
