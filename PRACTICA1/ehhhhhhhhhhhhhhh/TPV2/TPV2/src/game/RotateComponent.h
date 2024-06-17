#pragma once
#include "../ecs/Component.h"

class Transform;


class RotateComponent : public ecs::Component {
public:
	__CMPID_DECL__(ecs::cmp::ROTATECOMPONENT)

	RotateComponent();
	RotateComponent(float r);
	virtual ~RotateComponent();
	void initComponent() override;
	void update() override;
public:
	Transform* tr_;
	float rot_;

};