#pragma once //for header files

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "Spritesheet.h"
#include "Matrix.h"
#include "generic_functions.h"

#define y_gravity 0
#define x_gravity 0
#define PI 3.14159265358979323846
#define SHIP_FACING (-90 * PI ) / 180.0f

class Entity {
public:
	Entity();
	Entity(float, float, float);
	Entity(Spritesheet, float, float);

	void ssDraw();

	void Scale(float, float);
	void Translate(float, float);
	void Rotate(float);
	void buildMatrix();

	void setY(float);
	void setX(float);
	void setDirection(int);
	float getX();
	float getY();
	float getWidth();
	float getHeight();
	int getDirection();
	float getRotation();

	void setXVel(float);
	void setYVel(float);
	void setXAccel(float);
	void setYAccel(float);
	float getXVel();
	float getYVel();
	float getXAccel();
	float getYAccel();
	float getXFriction();
	float getYFriction();

	float rotation;
	float rAccel;
	float rVel;
	float rFriction;
	bool idle;
	bool collides;
	float facing;
	Matrix mat;
private:
	int direction;
	float x;
	float y;

	float scale_x;
	float scale_y;
	float translate_x;
	float translate_y;

	int numPoints;

	float xVel;
	float yVel;
	float xAccel;
	float yAccel;
	float xFriction;
	float yFriction;

	Spritesheet ss;
	float scale;

	float width;
	float height;
};