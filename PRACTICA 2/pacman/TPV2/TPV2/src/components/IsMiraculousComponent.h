#pragma once

#include "../ecs/Component.h"

struct IsMiraculousComponent : public ecs::Component
{
	__CMPID_DECL__(ecs::cmp::MIRACULOUS)

	IsMiraculousComponent();
	virtual ~IsMiraculousComponent();

	// true si esa fruta es miraculousa
	bool isMiraculous;

	// gestiona la trasformacion a la miraculousidad
	void MiraculousTransformation();

	// frecuencia N -> una fruta milagrosa se convierte en milagrosa cada N segundos
	uint32_t minNFrecuency = 10000; // 10 segundos
	uint32_t maxNFrecuency = 20000; // 20 segundos
	uint32_t NFrecuency; 

	// duracion M -> queda en estado milagroso durante M segundo
	uint32_t minMDuration = 1000; // 1 segundo
	uint32_t maxMDuration = 5000; // 5 segundos
	uint32_t MDuration;

	// cuando se activo el miraculous
	uint32_t timeOfActivation;
};