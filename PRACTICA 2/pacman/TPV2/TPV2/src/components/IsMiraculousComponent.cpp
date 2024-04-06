#include "IsMiraculousComponent.h"
#include "../sdlutils/SDLUtils.h"

IsMiraculousComponent::IsMiraculousComponent()
{
	// inicializa la N frecuency
	NFrecuency = sdlutils().rand().nextInt(minNFrecuency, maxNFrecuency);

	// lo resetea tiempo y tipo de fruta
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