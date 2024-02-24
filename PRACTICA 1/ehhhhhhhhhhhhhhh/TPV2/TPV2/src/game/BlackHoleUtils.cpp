#include "BlackHoleUtils.h"

#include "../sdlutils/SDLUtils.h"

BlackHoleUtils::BlackHoleUtils() :
	rand_(sdlutils().rand()), width_(sdlutils().width()), height_(
		sdlutils().height()) {


}

BlackHoleUtils::~BlackHoleUtils()
{
}

void BlackHoleUtils::create_blackholes(int n)
{

	// saca el centro de la pantalla
	const int centerX = sdlutils().width()/2;
	const int centerY = sdlutils().height()/2;

	// crea n agujeros negros
	for (int i = 0; i < n; i++) {
		int pos = rand_.nextInt(100, 300);	// posicion random (saca el radio)
		int rad = rand_.nextInt(5, 10);		// radio random para el agujero


		// saca la posicion el agujero segun la funcion del circulo y el angulo
		int x = 1;
		int y = 0;

	}

}
