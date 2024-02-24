#include "RotateComponent.h"
#include "Game.h"
#include "../components/Transform.h"
#include "../ecs/Manager.h"


RotateComponent::RotateComponent() : rot_(1)
{
}

RotateComponent::RotateComponent(float r) : rot_(r)
{
}

RotateComponent::~RotateComponent()
{
	tr_ = nullptr;
}

void RotateComponent::initComponent()
{
	tr_ = mngr_->getComponent<Transform>(ent_); // recibe el transform de la entidad en la que se encuentra
}

void RotateComponent::update()
{
	tr_->setRot(tr_->getRot() + rot_);
}
