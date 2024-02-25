#pragma once

#include "MissileFacade.h"
#include "src/ecs/ecs.h"

//class Vector2D;

class MissileUtils : public MissileFacade // hereda de su fachada
{
	// atributos privados
private:
	int size_ = 30; // tamanio de misil
	ecs::Manager* mngr; // referencia al manager

	// metodos publicos
public:
	MissileUtils();
	~MissileUtils() override = default;
	void create_missiles(int n) override;
	void remove_all_missiles() override;
};