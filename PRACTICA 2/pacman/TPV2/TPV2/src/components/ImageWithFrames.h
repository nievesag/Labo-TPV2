// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <SDL.h>

#include "../ecs/Component.h"

class Texture;
class Transform;

class ImageWithFrames: public ecs::Component {
public:

	__CMPID_DECL__(ecs::cmp::IMAGEWITHFRAMES)

	ImageWithFrames(Texture *tex, //
			int cols, int rows);
	virtual ~ImageWithFrames();
	void initComponent() override;
	void render() override;

	// parece que hacen lo mismo row/col pero no! 
	// --> recomendacion: para settear la posicion siempre con el setRow settea la fila y dejas el setCol a 0 (se pone solo)
	// luego para especificar la columna usas el offsetX  y dejas el offsetX en 0
	// !! solo funciona para frames en horizontal!
	// 
	// para ampiarlo y hacer que funcione en vertical habria que invertir todas las variables en el render y hacer un setRowFrames y ya
	//
	// Ejemplo (esta en el GhsotSys; changeGhosts):
	// img->setXoffset(6);			// ajusta el offset en X, es decir, la columna del sprite inicial
	// img->setRow(3);				// settea la fila inicial (empezando a contar desde el 0)
	// img->setColFrames(2);		// sette el numero de frames que tiene en horizontal

	void setRow(int row) { currFrameR_ = row; }
	void setCol(int col) { currFrameC_ = col; }

	// para poner el offset DE LOS FRAMES 
	void setXoffset(int x) { xoffset = x; }		// ajusta el offset en X, es decir, la columna del sprite inicial
	void setYoffset(int y) { yoffset = y; }		// ajusta el offset en Y, es decir, la fila del sprite inicial	

	void setColFrames(int col);					// settea el numero de frames que tiene la animacion

	void reset();

private:
	Texture *tex_;
	Transform *tr_;
	int currFrameR_;
	int currFrameC_;
	int x_;
	int y_;
	int w_;
	int h_;
	int srow_;
	int scol_;
	int nrow_;
	int ncol_;
	int frameWidth_;
	int frameHeight_;
	int xoffset;
	int yoffset;
	Uint32 lastFrameChange_;
};