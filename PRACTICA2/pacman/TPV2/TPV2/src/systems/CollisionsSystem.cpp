// This file is part of the course TPV2@UCM - Samir Genaim

#include "CollisionsSystem.h"

#include "../components/Transform.h"
#include "../ecs/Manager.h"
#include "../utils/Collisions.h"
#include "../components/IsMiraculousComponent.h"
#include "../components/IsInmuneComponent.h"

CollisionsSystem::CollisionsSystem() {}

CollisionsSystem::~CollisionsSystem() {}

void CollisionsSystem::initSystem() {}

void CollisionsSystem::update()
{
	checkCollisions(); // en cada update checkea todas las colisiones
}

void CollisionsSystem::checkCollisions()
{
	checkPacmanGhosts();
	checkPacmanFruit();
}

void CollisionsSystem::checkPacmanGhosts()
{
	auto pm = mngr_->getHandler(ecs::hdlr::PACMAN); // entidad pacman
	auto pTR = mngr_->getComponent<Transform>(pm); // transform de pacman

	auto& ghosts = mngr_->getEntities(ecs::grp::GHOSTS); // coge el grupo de entidades fantasmas
	auto n = ghosts.size(); // guarda el tamanio del grupo

	// recorre todos los fantasmas 
	for (auto i = 0u; i < n; i++) 
	{
		// guarda en e el fantasma que esta siendo evaluado en este momento
		auto e = ghosts[i];

		// si el fantasma esta vivo
		if (mngr_->isAlive(e)) 
		{ 
			auto gTR = mngr_->getComponent<Transform>(e); // transform del fantasma

			// checkea si pacman colisiona con fantasma
			if (Collisions::collides(			
				pTR->pos_, pTR->width_, pTR->height_, 
				gTR->pos_, gTR->width_, gTR->height_)) 
			{

				// ADMINISTRA MENSAJES A MANDAR SEGUN COLISION
				Message m;

				// si pacman no es inmune, muere pacman
				if(!(mngr_->getComponent<IsInmuneComponent>(mngr_->getHandler(ecs::hdlr::PACMAN))->isInmune))
				{
					m.id = _m_KILL_PACMAN; // manda mensaje de pacman muerto
				}

				// si pacman es inmune, muere el fantasma
				else
				{
					m.id = _m_EAT_GHOST; // manda mensaje de fantasma comido
					m.eat_ghost_data.e = e; // guarda fantasma colisionado
				}

				mngr_->send(m);
			}
		}
	}
}

void CollisionsSystem::checkPacmanFruit()
{
	auto pm = mngr_->getHandler(ecs::hdlr::PACMAN); // entidad pacman
	auto pTR = mngr_->getComponent<Transform>(pm); // transform de pacman

	auto& fruits = mngr_->getEntities(ecs::grp::FRUITS); // coge el grupo de entidades frutas
	auto n = fruits.size(); // guarda el tamanio del grupo

	// recorre todas las frutas
	for (auto i = 0u; i < n; i++) 
	{
		// guarda en e la fruta que esta siendo evaluada en este momento
		auto e = fruits[i];

		// si existe la fruta (por si acaba de ser eliminada)
		if (mngr_->isAlive(e)) 
		{
			auto fTR = mngr_->getComponent<Transform>(e); // transform de la fruta

			// checkea si pacman colisiona con fruta
			if (Collisions::collides(	
				pTR->pos_, pTR->width_, pTR->height_, 
				fTR->pos_, fTR->width_, fTR->height_)) 
			{
				// manda mensaje de colision
				Message m;
				m.id = _m_PACMAN_FOOD_COLLISION;
				m.eat_fruit_data.e = e;
				mngr_->send(m);
			}
		}
	}
}