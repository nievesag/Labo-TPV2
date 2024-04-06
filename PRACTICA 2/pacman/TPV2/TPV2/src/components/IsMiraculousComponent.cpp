#include "IsMiraculousComponent.h"
#include "../sdlutils/SDLUtils.h"

IsMiraculousComponent::IsMiraculousComponent()
{
	minNFrecuency = 10000; // 10 segundos
	maxNFrecuency = 20000; // 20 segundos

	// duracion M -> queda en estado milagroso durante M segundo
	minMDuration = 1000; // 1 segundo
	maxMDuration = 5000; // 5 segundos

	// inicializa la N frecuency
	NFrecuency = sdlutils().rand().nextInt(minNFrecuency, maxNFrecuency);

	// inicializa variables miembro
	timeOfActivation = sdlutils().virtualTimer().currTime();
	isMiraculous = false;
}

IsMiraculousComponent::~IsMiraculousComponent() {}

void IsMiraculousComponent::MiraculousTransformation()
{
	// guarda el tiempo en el que lo has activado
	timeOfActivation = sdlutils().virtualTimer().currTime();

	// establece cuanto tiempo va a estar en ese estado
	MDuration = sdlutils().rand().nextInt(minMDuration, maxMDuration);

	// es miraculousa a true
	isMiraculous = true;
}

void IsMiraculousComponent::resetIsMiraculous()
{
	// es miraculousa a false
	isMiraculous = false;
}