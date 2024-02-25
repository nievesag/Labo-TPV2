#pragma once

#include "MissileFacade.h"

class Vector2D;

class MissileUtils : public MissileFacade
{
public:
	MissileUtils();
	virtual ~MissileUtils();
	void remove_all_missiles() override;
	void create_missile(int n) override;

private:
	void generateBlackHole(const Vector2D& p, int r, int g);
	int size_ = 30;
	ecs::Manager* mngr;
};