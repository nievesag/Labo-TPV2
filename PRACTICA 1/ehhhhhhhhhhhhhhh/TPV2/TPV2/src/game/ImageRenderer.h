// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include "RenderComponent.h"

class Texture;

class ImageRenderer: public RenderComponent {
public:
	//__CMPID_DECL__(ecs::cmp::IMAGERENDERER)

	ImageRenderer(Texture *img);
	virtual ~ImageRenderer();
	void render(Container *o) override;
private:
	Texture *img_;
};

