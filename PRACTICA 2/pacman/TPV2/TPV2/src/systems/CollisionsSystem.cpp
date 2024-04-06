// This file is part of the course TPV2@UCM - Samir Genaim

#include "CollisionsSystem.h"

#include "../components/Transform.h"
#include "../ecs/Manager.h"
#include "../utils/Collisions.h"
#include "../components/IsMiraculousComponent.h"

CollisionsSystem::CollisionsSystem() {
	// TODO Auto-generated constructor stub

}

CollisionsSystem::~CollisionsSystem() {
	// TODO Auto-generated destructor stub
}

void CollisionsSystem::initSystem() {
}

void CollisionsSystem::update() {

	checkCollisions();

}

void CollisionsSystem::checkCollisions()
{
	checkPacmanGhosts();
	checkPacmanFruit();

}

void CollisionsSystem::checkPacmanGhosts()
{
	// the PacMan's Transform
	//
	auto pm = mngr_->getHandler(ecs::hdlr::PACMAN);
	auto pTR = mngr_->getComponent<Transform>(pm);

	// For safety, we traverse with a normal loop until the current size. In this
	// particular case we could use a for-each loop since the list stars is not
	// modified.
	//
	auto& ghosts = mngr_->getEntities(ecs::grp::GHOSTS);
	auto n = ghosts.size();
	for (auto i = 0u; i < n; i++) {
		auto e = ghosts[i];
		if (mngr_->isAlive(e)) { // if the ghost is active (it might have died in this frame)

			// the PacMan Transform
			auto gTR = mngr_->getComponent<Transform>(e);

			// check if PacMan collides with the ghost 
			if (Collisions::collides(			
				pTR->pos_, pTR->width_, pTR->height_, 
				gTR->pos_, gTR->width_, gTR->height_)) {

				Message m;
				m.id = _m_KILL_PACMAN;
				m.kill_pacman_data.e = e;
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
				m.id = _m_EAT_FRUIT;
				m.eat_fruit_data.e = e;
				mngr_->send(m);

				//if(mngr_->getComponent<IsMiraculousComponent>(e)->isMiraculous)
				//{
				//	Message m;
				//	m.id = _m_IMMUNITY_START;
				//	m.eat_fruit_data.e = e;
				//	mngr_->send(m);
				//}
			}
		}
	}
}