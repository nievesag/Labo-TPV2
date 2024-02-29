#pragma once

#include "MissileFacade.h"
#include "../ecs/ecs.h"
#include "../sdlutils/RandomNumberGenerator.h"

//class Vector2D;

class MissileUtils : public MissileFacade // hereda de su fachada
{
	// atributos privados
private:
	// tamanio de misil
	int size_w = 40;
	int size_h = 20;

	// deben ser inicializados en el cpp de la constructora
	ecs::Manager* mngr; // referencia al manager
	RandomNumberGenerator& rand_; // para esquina random

	// metodos publicos
public:
	MissileUtils();
	~MissileUtils() override = default;
	void create_missiles(int n) override;
	void remove_all_missiles() override;
};