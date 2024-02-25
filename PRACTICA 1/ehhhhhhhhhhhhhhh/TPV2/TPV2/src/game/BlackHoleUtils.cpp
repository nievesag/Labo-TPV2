#include "BlackHoleUtils.h"

#include "../sdlutils/SDLUtils.h"
#include "../ecs/Manager.h"
#include "ImageRenderer.h"
#include "../components/Image.h"
#include "../components/Transform.h"
#include "RotateComponent.h"
#include "Game.h"

BlackHoleUtils::BlackHoleUtils() :
	rand_(sdlutils().rand()),
	mngr(Game::instance()->getMngr())
{
	
}

BlackHoleUtils::~BlackHoleUtils()
{

}

void BlackHoleUtils::remove_all_blackholes()
{

}

void BlackHoleUtils::create_blackholes(int n)
{
	// saca el centro de la pantalla
	const int centerX = sdlutils().width()/2;
	const int centerY = sdlutils().height()/2;
	const double alpha = 360.0 / n;

	// crea n agujeros negros
	for (int i = 0; i < n; i++) {
		int rad = rand_.nextInt(100, 300);	// posicion random (saca el radio)
		int rot = rand_.nextInt(5, 10);		// radio random para el agujero

		// saca la posicion el agujero segun la funcion del circulo y el angulo
		int x = centerX + rad * cos(alpha*i);
		int y = centerY + rad * sin(alpha * i);


		// añade una entidad al grupo BLACKHOLE
		auto a = mngr->addEntity(ecs::grp::BLACKHOLE);


		// componente transform
		mngr->addComponent<Transform>(a,				// obj
									Vector2D(x, y),		// pos
									Vector2D(0, 0),		// vel
									size_,				// w
									size_,				// h
									0.0);				// rot
									
		// componente para que se renderice
		//mngr->addComponent<ImageRenderer>(a, &sdlutils().images().at("black_hole"));
		mngr->addComponent<Image>(a, &sdlutils().images().at("black_hole"));

		// componente para que rote
		mngr->addComponent<RotateComponent>(a, rot);
	}
}