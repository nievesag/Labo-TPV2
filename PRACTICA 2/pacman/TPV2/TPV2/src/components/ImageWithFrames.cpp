// This file is part of the course TPV2@UCM - Samir Genaim

#include "ImageWithFrames.h"

#include <string>
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../sdlutils/macros.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/Texture.h"
#include "Transform.h"

ImageWithFrames::ImageWithFrames(Texture *tex, int rows, int cols) :
		tex_(tex), 
		tr_(nullptr), 
		currFrameR_(0), 
		currFrameC_(0), 
		lastFrameChange_(0)
{

	// dimensiones de los frames
	frameWidth_ = tex_->width() / cols;
	frameHeight_ = tex->height() / rows;

	// numero de columnas y filas del spritesheet
	ncol_ = cols;
	nrow_ = rows;


	xoffset = 0;
	yoffset = 0;
}

ImageWithFrames::~ImageWithFrames() {}

void ImageWithFrames::initComponent() {
	tr_ = mngr_->getComponent<Transform>(ent_);
	assert(tr_ != nullptr);
}

void ImageWithFrames::render() {

	// cada 50 cambia al siguiente frame
	if (sdlutils().virtualTimer().currTime() > lastFrameChange_ + 50) {
		// iguala el tiempo actual a lastFrameChange
		lastFrameChange_ = sdlutils().virtualTimer().currTime();
		// filas circulares <3
		currFrameC_ = (currFrameC_ + 1) % ncol_;
	}


	// current frame +  offset
	int c = (currFrameC_ % ncol_) + xoffset;

	// rect source			       lados				altura
	auto src = build_sdlrect(c * frameWidth_, (currFrameR_ + yoffset) * frameHeight_, frameWidth_, frameHeight_);
		// c*frameWidth_ saca la posicion x del sprite que queremos
		// lo mismo para  (currFrameR_ + yoffset) * frameHeight_ pero con y

	// rect destino
	auto dest = build_sdlrect(tr_->getPos(), tr_->getWidth(), tr_->getHeight());
	
	// render
	tex_->render(src, dest, tr_->getRot());
}

void ImageWithFrames::setColFrames(int col)
{
	ncol_ = col;
}

void ImageWithFrames::reset()
{
	xoffset = 0;
	yoffset = 0;
}