#pragma once

#include "MissileFacade.h"
#include "src/ecs/ecs.h"
#include "src/sdlutils/RandomNumberGenerator.h"

//class Vector2D;

class MissileUtils : public MissileFacade // hereda de su fachada
{
	// atributos privados
private:
	int size_ = 30; // tamanio de misil

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