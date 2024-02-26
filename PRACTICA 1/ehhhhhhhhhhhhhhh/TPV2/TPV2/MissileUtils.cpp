#include "MissileUtils.h"

#include "src/sdlutils/SDLUtils.h"
#include "src/ecs/Manager.h"
#include "src/components/Image.h"
#include "src/components/Transform.h"
#include "src/game/ImageRenderer.h"
#include "src/components/Follow.h" 
#include "src/game/Game.h"

MissileUtils::MissileUtils()
	: mngr(Game::instance()->getMngr()), rand_(sdlutils().rand())
{

}

void MissileUtils::create_missiles(int n)
{
	// sacar las 4 esquinas ?

	for(int i = 0; i < n; i++)
	{
		int corner = rand_.nextInt(0, 3);	// saca esquina random -> 0/arriba izq ; 1/arriba der ; 2/abajo izq ; 3/abajo der

		if (corner == 0) {

		}
		else if (corner == 1) {

		}
		else if (corner == 2) {

		}
		else if (corner == 3) {

		}

		// añade una entidad al grupo MISSILE
		auto missile = mngr->addEntity(ecs::grp::MISSILE);

		// componente transform
		/*mngr->addComponent<Transform> (missile,				// obj a aniadir
									   Vector2D(x, y),			// pos (depende de la esquina)
									   Vector2D(0, 0),			// vel en x e y (depende del jugador -> le sigue)
									   size_,					// tamanio w // !!!!!!!! deben ser diferentes -> TOCAR ESTO
									   size_,					// tamanio h
									   0.0);					// rotacion 
		*/

		// componente para que se renderice
		mngr->addComponent<Image>(missile, &sdlutils().images().at("black_hole"));

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

}