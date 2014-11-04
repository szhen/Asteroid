#include "entity.h"

Entity::Entity() {
	x = 0.0f;
	y = 0.0f;
}

Entity::Entity(float x, float y, float rot) : x(x), y(y), rotation(rot) {
	xVel = 0.0f;
	yVel = 0.0f;
	xAccel = 0.0f;
	yAccel = 0.0f;
	xFriction = 2.0f;
	yFriction = 2.0f;
}

Entity::Entity(Spritesheet tmp, float x, float y) {
	ss = tmp;
	Entity::x = x;
	Entity::y = y;
	width = tmp.getWidth();
	height = tmp.getHeight();
	xVel = 0.0f;
	yVel = 0.0f;
	xAccel = 0.0f;
	yAccel = 0.0f;
	xFriction = 2.0f;
	yFriction = 2.0f;
	scale = 1.0f;
	scale_x = 1.0f;
	scale_y = 1.0f;
	rotation = 0.0f;
	rAccel = 0.0f;
	rVel = 0.0f;
	rFriction = 1.0f;
	idle = true;
	collides = false;
	facing = (-90 * PI) / 180.0f;
}

void Entity::ssDraw() {
	buildMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(mat.ml);
	ss.draw(scale);
	glPopMatrix();
}

void Entity::Scale(float x, float y) {
	scale_x = x;
	scale_y = y;
}

void Entity::Translate(float x, float y) {
	translate_x = x;
	translate_y = y;
}

void Entity::Rotate(float rad) {
	rotation = (rad * PI ) / 180.0f;
}


void Entity::buildMatrix() {
	Matrix translate;
	Matrix scale;
	Matrix rotate;

	// translate
	translate.m[3][0] = x;
	translate.m[3][1] = y;

	// scale
	scale.m[0][0] = scale_x;
	scale.m[1][1] = scale_y;

	// rotate
	rotate.m[0][0] = cos(rotation + facing);
	rotate.m[1][0] = -sin(rotation + facing);
	rotate.m[0][1] = sin(rotation + facing);
	rotate.m[1][1] = cos(rotation + facing);

	mat = scale * rotate * translate;
}

float Entity::getRotation() { return rotation; }

void Entity::setX(float newX) {	x = newX; }

void Entity::setY(float newY) {	y = newY; }

void Entity::setDirection(int dire) { direction = dire; }

float Entity::getX() { return x; }

float Entity::getY() { return y; }

float Entity::getWidth() { return width; }

float Entity::getHeight() { return height; }

int Entity::getDirection() { return direction; }

void Entity::setXVel(float newVel) { xVel = newVel; }
void Entity::setYVel(float newVel) { yVel = newVel; }
void Entity::setXAccel(float newAccel) { xAccel = newAccel; }
void Entity::setYAccel(float newAccel) { yAccel = newAccel; }
float Entity::getXVel() { return xVel; }
float Entity::getYVel() { return yVel; }
float Entity::getXAccel() { return xAccel; }
float Entity::getYAccel() { return yAccel; }
float Entity::getXFriction() { return xFriction; }
float Entity::getYFriction() { return yFriction; }