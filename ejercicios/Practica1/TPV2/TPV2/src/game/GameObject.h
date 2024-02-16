// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once


#include <SDL.h>

#include "../utils/Vector2D.h"

class GameObject {
public:
	GameObject() :
			pos_(), vel_(), width_(), height_(), rot_() {
		rot_ = 0.0f;
	}
	virtual ~GameObject() {
	}

	virtual void handleInput() = 0;
	virtual void update() = 0;
	virtual void render() = 0;

	inline float getHeight() const {
		return height_;
	}

	inline void setHeight(float height) {
		height_ = height;
	}

	inline Vector2D& getPos() {
		return pos_;
	}

	inline Vector2D& getVel() {
		return vel_;
	}

	inline float getWidth() const {
		return width_;
	}

	inline void setWidth(float width) {
		width_ = width;
	}

	inline float getRotation() {
		return rot_;
	}

	inline void setRotation(float rotation) {
		rot_ = rotation;
	}


protected:

	Vector2D pos_;
	Vector2D vel_;
	float width_;
	float height_;
	double rot_;
};

