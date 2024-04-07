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
	checkCollisions();
}

void CollisionsSystem::checkCollisions()
{
	// checkea todas las posibles colisiones
	checkPacmanGhosts();
	checkPacmanFruit();
}

void CollisionsSystem::checkPacmanGhosts()
{
	// the PacMan's Transform
	auto pm = mngr_->getHandler(ecs::hdlr::PACMAN);
	auto pTR = mngr_->getComponent<Transform>(pm);

	// recorre todos los fantasmas 
	auto& ghosts = mngr_->getEntities(ecs::grp::GHOSTS);
	auto n = ghosts.size();
	for (auto i = 0u; i < n; i++) {
		auto e = ghosts[i];

		// if the ghost is active (it might have died in this frame)
		if (mngr_->isAlive(e)) { 

			// the PacMan Transform
			auto gTR = mngr_->getComponent<Transform>(e);

			// check if PacMan collides with the ghost 
			if (Collisions::collides(			
				pTR->pos_, pTR->width_, pTR->height_, 
				gTR->pos_, gTR->width_, gTR->height_)) 
			{
				Message m;

				// si pacman no es inmune, muere pacman
				if(!(mngr_->getComponent<IsInmuneComponent>(mngr_->getHandler(ecs::hdlr::PACMAN))->isInmune))
				{
					m.id = _m_KILL_PACMAN; // manda mensaje de pacman muerto
					m.kill_pacman_data.e = e;
				}

				// si pacman es inmune, muere el fantasma
				else
				{
					m.id = _m_EAT_GHOST; // manda mensaje de fantasma comido
					m.eat_ghost_data.e = e;
				}

				mngr_->send(m);
			}
		}
	}
}

void CollisionsSystem::checkPacmanFruit()
{
	// the PacMan's Transform
	auto pm = mngr_->getHandler(ecs::hdlr::PACMAN);
	auto pTR = mngr_->getComponent<Transform>(pm);

	// recorre todas las frutas
	auto& fruits = mngr_->getEntities(ecs::grp::FRUITS);
	auto n = fruits.size();
	for (auto i = 0u; i < n; i++) {
		auto e = fruits[i];
		if (mngr_->isAlive(e)) { // if the fruit is active (it might have died in this frame)

			auto gTR = mngr_->getComponent<Transform>(e);

			// check if PacMan collides with the fruit 
			if (Collisions::collides(	
				pTR->pos_, pTR->width_, pTR->height_, 
				gTR->pos_, gTR->width_, gTR->height_)) {

				Message m;
				m.id = _m_EAT_FRUIT; // manda mensaje de fruta comida
				m.eat_fruit_data.e = e;
				mngr_->send(m);

				// INMUNIDAD
				if (mngr_->getComponent<IsMiraculousComponent>(e) != nullptr) 
				{
					// si la fruta con la que colisiones es miracoulosa empieza la inmunidad de pacman
					if(mngr_->getComponent<IsMiraculousComponent>(e)->isMiraculous)
					{
						Message m;
						m.id = _m_IMMUNITY_START;
						m.eat_fruit_data.e = e;
						mngr_->send(m);
					}
				}
			}
		}
	}
}