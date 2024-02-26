#include "MissileUtils.h"

#include "../sdlutils/SDLUtils.h"
#include "../ecs/Manager.h"
#include "../components/Image.h"
#include "../components/Transform.h"
#include "../game/ImageRenderer.h"
#include "../components/Follow.h" 
#include "../game/Game.h"

MissileUtils::MissileUtils()
	: mngr(Game::instance()->getMngr()), rand_(sdlutils().rand())
{

}

void MissileUtils::create_missiles(int n)
{
	// dafault
	int x = 0; 
	int y = 0; 

	for (int i = 0; i < n; i++)
	{
		int corner = rand_.nextInt(0, 3);	// saca esquina random -> 0/arriba izq ; 1/arriba der ; 2/abajo izq ; 3/abajo der

		if (corner == 0) {
			x = 0;
			y = 0;
		}
		else if (corner == 1) {
			x = sdlutils().width() - size_;
			y = 0;
		}
		else if (corner == 2) {
			x = 0;
			y = sdlutils().height() - size_;
		}
		else if (corner == 3) {
			x = sdlutils().width() - size_;
			y = sdlutils().height() - size_;
		}

		// añade una entidad al grupo MISSILE
		auto missile = mngr->addEntity(ecs::grp::MISSILE);

		// componente transform
		mngr->addComponent<Transform> (missile,					// obj a aniadir
									   Vector2D(x, y),			// pos (depende de la esquina)
									   Vector2D(0, 0),			// vel en x e y (depende del jugador -> le sigue)
									   size_,					// tamanio w // !!!!!!!! deben ser diferentes -> TOCAR ESTO
									   size_,					// tamanio h
									   0.0);					// rotacion 

		std::cout << x << " " << y;

		// componente para que se renderice
		mngr->addComponent<Image>(missile, &sdlutils().images().at("missle"));

		// accede a la entidad fighter
		const entity_t fighter = mngr->getHandler(ecs::hdlr::FIGHTER);

		// guarda la posicion del fighter
		Vector2D& fighterPos = mngr->getComponent<Transform>(fighter)->getPos();

		// componente para que siga al jugador
		mngr->addComponent<Follow>(missile, fighterPos); // le pasas entidad a seguir y la posicion de a quien siga
	}
}

void MissileUtils::remove_all_missiles()
{
	auto mngr = Game::instance()->getMngr();
	for (auto e : mngr->getEntities(ecs::grp::MISSILE)) {
		mngr->setAlive(e, false);
	}
	mngr->refresh();
}