// This file is part of the course TPV2@UCM - Samir Genaim

#include "RunningState.h"

#include "../components/Gun.h"
#include "../components/Transform.h"
#include "../ecs/Manager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Collisions.h"
#include "AsteroidsFacade.h"
#include "FighterFacade.h"
#include "BlackHoleFacade.h"
#include "MissileFacade.h"

#include "Game.h"

RunningState::RunningState(AsteroidsFacade *ast_mngr,
		FighterFacade *fighter_mngr, BlackHoleFacade *blackhole_mngr, MissileFacade *missile_mngr) :
		ihdlr(ih()), //
		ast_mngr_(ast_mngr), //
		fighter_mngr_(fighter_mngr), //
		blackhole_mngr_(blackhole_mngr), 
		missile_mngr_(missile_mngr),//
		lastTimeGeneratedAsteroids_() {
}

RunningState::~RunningState() {
}

void RunningState::leave() {
}

void RunningState::update() {

	auto mngr = Game::instance()->getMngr();

	// check if fighter won
	if (mngr->getEntities(ecs::grp::ASTEROIDS).size() == 0) {
		Game::instance()->setState(Game::GAMEOVER);
		return;
	}

	// move to pause if P pressed
	if (ihdlr.keyDownEvent() && ihdlr.isKeyDown(SDL_SCANCODE_P)) {
		Game::instance()->setState(Game::PAUSED);
		return;
	}

	auto fighter = mngr->getHandler(ecs::hdlr::FIGHTER);
	auto &asteroids = mngr->getEntities(ecs::grp::ASTEROIDS);
	auto &blackholes = mngr->getEntities(ecs::grp::BLACKHOLE);
	auto &missiles = mngr->getEntities(ecs::grp::MISSILE);

	// update
	mngr->update(fighter);
	for (auto a : asteroids) {
		mngr->update(a);
	}

	for (auto a : blackholes) {
		mngr->update(a);
	}

	for (auto a : missiles) {

		// obtener transform de cada misil
		auto mTR = mngr->getComponent<Transform>(a);

		// obtener velocidad de cada misil
		auto mVel = mTR->getVel();

		// actualizar rotacion de cada misil
		float mRot = Vector2D(0, -1).angle(mVel);
		mTR->setRot(mRot);

		// check si han salido de la pantalla
		int mPosX = mTR->getPos().getX();
		int mPosY = mTR->getPos().getY();
		if (mPosX + mTR->getWidth() < 0 || mPosX > sdlutils().width() || mPosY + mTR->getHeight() < 0 || mPosY > sdlutils().height()) {
			mngr->setAlive(a, false);
		}

		mngr->update(a);
	}

	// check collisions
	checkCollisions();

	// render
	sdlutils().clearRenderer();
	for (auto a : asteroids) {
		mngr->render(a);
	}
	for (auto a : blackholes) {
		mngr->render(a);
	}
	for (auto a : missiles) {
		mngr->render(a);
	}
	mngr->render(fighter);
	sdlutils().presentRenderer();

	mngr->refresh();

	if (sdlutils().virtualTimer().currTime() > lastTimeGeneratedAsteroids_ + 5000) {
		ast_mngr_->create_asteroids(1);
		lastTimeGeneratedAsteroids_ = sdlutils().virtualTimer().currTime();
	}

	// genera misiles cada 15 segundos
	if (sdlutils().virtualTimer().currTime() > lastTimeGeneratedMissiles_ + 15000) {
		missile_mngr_->create_missiles(1);
		lastTimeGeneratedMissiles_ = sdlutils().virtualTimer().currTime();
	}
}

void RunningState::enter() {
	lastTimeGeneratedAsteroids_ = sdlutils().virtualTimer().currTime();
	lastTimeGeneratedMissiles_ = sdlutils().virtualTimer().currTime();
}

void RunningState::checkCollisions() {
	// game manager
	auto mngr = Game::instance()->getMngr();

	// fighter
	auto fighter = mngr->getHandler(ecs::hdlr::FIGHTER);
	auto fighterTR = mngr->getComponent<Transform>(fighter);
	auto fighterGUN = mngr->getComponent<Gun>(fighter);

	// asteroids
	auto& asteroids = mngr->getEntities(ecs::grp::ASTEROIDS);

	// blackhole
	auto& blackhole = mngr->getEntities(ecs::grp::BLACKHOLE);

	// missiles
	auto& missiles = mngr->getEntities(ecs::grp::MISSILE);

	// asteroids
	auto num_of_asteroids = asteroids.size();
	for (auto i = 0u; i < num_of_asteroids; i++) {
		auto a = asteroids[i];
		if (!mngr->isAlive(a))
			continue;

		// asteroid with fighter
		auto aTR = mngr->getComponent<Transform>(a);
		if (Collisions::collidesWithRotation( //
			fighterTR->getPos(), //
			fighterTR->getWidth(), //
			fighterTR->getHeight(), //
			fighterTR->getRot(), //
			aTR->getPos(), //
			aTR->getWidth(), //
			aTR->getHeight(), //
			aTR->getRot())) {
			onFigherDeath();
			return;
		}

		// asteroid with bullets
		for (Gun::Bullet& b : *fighterGUN) {
			if (b.used) {
				if (Collisions::collidesWithRotation( //
					b.pos, //
					b.width, //
					b.height, //
					b.rot, //
					aTR->getPos(), //
					aTR->getWidth(), //
					aTR->getHeight(), //
					aTR->getRot())) {
					ast_mngr_->split_astroid(a);
					b.used = false;
					sdlutils().soundEffects().at("explosion").play();
					continue;
				}
			}
		}

		// asteroids with blackholes
		for (auto b : blackhole) {
			auto bhTR = mngr->getComponent<Transform>(b);
			if (Collisions::collidesWithRotation( //
				bhTR->getPos(), //
				bhTR->getWidth(), //
				bhTR->getHeight(), //
				bhTR->getRot(), //
				aTR->getPos(), //
				aTR->getWidth(), //
				aTR->getHeight(), //
				aTR->getRot())) {
				ast_mngr_->change_asteroid_pos(a);
				sdlutils().soundEffects().at("explosion").play();
				continue;
			}
		}

		// asteroids with missiles
		for (auto m : missiles) {
			auto msTR = mngr->getComponent<Transform>(m);
			if (Collisions::collidesWithRotation( //
				msTR->getPos(), //
				msTR->getWidth(), //
				msTR->getHeight(), //
				msTR->getRot(), //
				aTR->getPos(), //
				aTR->getWidth(), //
				aTR->getHeight(), //
				aTR->getRot())) {
				ast_mngr_->split_astroid(a);
				sdlutils().soundEffects().at("explosion").play();
				continue;
			}
		}
	}

	// blackholes
	auto num_of_blackholes = blackhole.size();
	for (auto i = 0u; i < num_of_blackholes; i++) {
		auto bh = blackhole[i];
		if (!mngr->isAlive(bh))
			continue;

		// blackhole with fighter
		auto bhTR = mngr->getComponent<Transform>(bh);
		if (Collisions::collidesWithRotation( //
			fighterTR->getPos(), //
			fighterTR->getWidth(), //
			fighterTR->getHeight(), //
			fighterTR->getRot(), //
			bhTR->getPos(), //
			bhTR->getWidth(), //
			bhTR->getHeight(), //
			bhTR->getRot())) {
			onFigherDeath();
			return;
		}
	}

	// missiles
	auto num_of_missiles = missiles.size();
	for (auto i = 0u; i < num_of_missiles; i++) {
		auto ms = missiles[i];
		if (!mngr->isAlive(ms)) {
			continue;
		}

		// missile with fighter
		auto msTR = mngr->getComponent<Transform>(ms);
		if (Collisions::collidesWithRotation( //
			fighterTR->getPos(), //
			fighterTR->getWidth(), //
			fighterTR->getHeight(), //
			fighterTR->getRot(), //
			msTR->getPos(), //
			msTR->getWidth(), //
			msTR->getHeight(), //
			msTR->getRot())) {
			onFigherDeath();
			return;
		}

		// missile with bullets
		for (Gun::Bullet& b : *fighterGUN) {
			if (b.used) {
				if (Collisions::collidesWithRotation(
					b.pos,
					b.width,
					b.height,
					b.rot,
					msTR->getPos(),
					msTR->getWidth(),
					msTR->getHeight(),
					msTR->getRot())) {
					mngr->setAlive(ms, false);
					b.used = false;
					sdlutils().soundEffects().at("explosion").play();
					continue;
				}
			}
		}
	}
}

void RunningState::onFigherDeath() {
	sdlutils().soundEffects().at("explosion").play();
	if (fighter_mngr_->update_lives(-1) > 0)
		Game::instance()->setState(Game::NEWROUND);
	else
		Game::instance()->setState(Game::GAMEOVER);
}
