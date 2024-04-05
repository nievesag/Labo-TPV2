#pragma once

#include "../ecs/Component.h"

class IsMiraculousComponent : public ecs::Component
{
	__CMPID_DECL__(ecs::cmp::MIRACULOUS)

	IsMiraculousComponent();
	virtual ~IsMiraculousComponent();

	void initComponent() override;
	void render() override;
};