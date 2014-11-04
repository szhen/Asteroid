#include "Application.h"
using namespace std;

Application::Application() {
	Init();
}

Application::~Application() {
	SDL_Quit();
}

void Application::Init() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Asteroids", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-2.66, 2.66, -2.0, 2.0, -2.0, 2.0);
	glMatrixMode(GL_MODELVIEW);

	srand(time(NULL));
	lastFrameTicks = 0.0f;
	timeLeftOver = 0.0f;
	enemyIndex = 0;
	bulletIndex = 0;
	shootCD = 0.2f;
	keys = SDL_GetKeyboardState(NULL);
	playerFacing = direction::RIGHT;
	Spritesheet tmp(LoadTexture("sheet.png", GL_RGBA), 0.0f / 1024.0f, 941.0f / 1024.0f, 113.0f / 1024.0f, 75.0f / 1024.0f);
	ship = Entity(tmp, 0.0f, 0.0f);
	Spritesheet tmpRock(LoadTexture("sheet.png", GL_RGBA), 0.0f / 1024.0f, 520.0f / 1024.0f, 120.0f / 1024.0f, 98.0f / 1024.0f);
	for (int i = 0; i < ROCKS; ++i) {
		rocks[i] = Entity(tmpRock, randomNumber(-2.66f, 2.66f), randomNumber(-2.0f, 2.0f));
		rocks[i].setXVel(randomNumber(-0.25f, 0.25f));
		rocks[i].setYVel(randomNumber(-0.25f, 0.25f));
		rocks[i].rVel = randomNumber(-1.0f, 1.0f);
	}
}

bool Application::UpdateAndRender() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			return true;
		}
	}
	// timestep 
	float fixedElapsed = elapsed + timeLeftOver;
	if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) {
		fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
	}
	while (fixedElapsed >= FIXED_TIMESTEP) {
		fixedElapsed -= FIXED_TIMESTEP;
		FixedUpdate();
		update(elapsed);
	}

	timeLeftOver = fixedElapsed;

	render();
	return false;
}

void Application::update(float elapsed) {
	ship.setXAccel(0.0f);
	ship.setYAccel(0.0f);
	ship.rAccel = 0.0f;
	if (keys[SDL_SCANCODE_RIGHT]) {
		ship.idle = false;
		ship.rAccel = -2.5;
	}
	else if (keys[SDL_SCANCODE_LEFT]) {
		ship.idle = false;
		ship.rAccel = 2.5f;
	}
	else {
		ship.idle = true;
	}
	if (keys[SDL_SCANCODE_UP]) {
		ship.idle = false;
		ship.setXAccel(cos(ship.getRotation())*2.5f);
		ship.setYAccel(sin(ship.getRotation())*2.5f);
	}
	// update player
	ship.setX(ship.getX() + ship.getXVel() * FIXED_TIMESTEP);
	ship.setY(ship.getY() + ship.getYVel() * FIXED_TIMESTEP);
	// update rocks
	for (int i = 0; i < ROCKS; ++i) {
		rocks[i].setX(rocks[i].getX() + rocks[i].getXVel() * FIXED_TIMESTEP);
		rocks[i].setY(rocks[i].getY() + rocks[i].getYVel() * FIXED_TIMESTEP);
	}
}

void Application::FixedUpdate() {
	ship.setXVel(lerp(ship.getXVel(), 0.0f, FIXED_TIMESTEP * ship.getXFriction()));
	ship.setYVel(lerp(ship.getYVel(), 0.0f, FIXED_TIMESTEP * ship.getYFriction()));
	ship.rVel = lerp(ship.rVel, 0.0f, FIXED_TIMESTEP * ship.rFriction);
	ship.rVel += ship.rAccel * FIXED_TIMESTEP;
	ship.setXVel(ship.getXVel() + ship.getXAccel() * FIXED_TIMESTEP);
	ship.setYVel(ship.getYVel() + ship.getYAccel() * FIXED_TIMESTEP);
	ship.setXVel(ship.getXVel() + x_gravity * FIXED_TIMESTEP);
	ship.setYVel(ship.getYVel() + y_gravity * FIXED_TIMESTEP);
	ship.rotation += ship.rVel * FIXED_TIMESTEP;
	for (int i = 0; i < ROCKS; ++i) {
		rocks[i].rotation += rocks[i].rVel* FIXED_TIMESTEP;
	}

	// player to rock
	for (int i = 0; i < ROCKS; ++i) {
		if (checkCollision(&ship, &rocks[i])) {
			collisionResponse(&ship, &rocks[i]);
			rocks[i].setXVel(randomNumber(-0.25f, 0.25f));
			rocks[i].setYVel(randomNumber(-0.25f, 0.25f));
		}
	}
	// rock to player
	for (int i = 0; i < ROCKS; ++i) {
		if (checkCollision(&rocks[i], &ship)) {
			collisionResponse(&rocks[i], &ship);
			rocks[i].setXVel(randomNumber(-0.25f, 0.25f));
			rocks[i].setYVel(randomNumber(-0.25f, 0.25f));
		}
	}
	// rock to rock
	for (int i = 0; i < ROCKS; ++i) {
		for (int j = 0; j < ROCKS; ++j) {
			if (i != j) {
				if (checkCollision(&rocks[i], &rocks[j])) {
					collisionResponse(&rocks[i], &rocks[i+1]);
					rocks[i].setXVel(randomNumber(-0.25f, 0.25f));
					rocks[i].setYVel(randomNumber(-0.25f, 0.25f));
				}
			}
		}
	}

	// wrapping
	// ship
	if (ship.getX() > 2.66f + 0.1f)
		ship.setX(-2.66f - 0.05f);
	else if (ship.getX() < -2.66f - 0.1f)
		ship.setX(2.66f + 0.05f);

	if (ship.getY() > 2.0f + 0.1f)
		ship.setY(-2.0f - 0.05f);
	else if (ship.getY() < -2.0f - 0.1f)
		ship.setY(2.0f + 0.05f);
	// rocks
	for (int i = 0; i < ROCKS; ++i) {
		if (rocks[i].getX() > 2.66f + 0.1f)
			rocks[i].setX(-2.66f - 0.05f);
		else if (rocks[i].getX() < -2.66f - 0.1f)
			rocks[i].setX(2.66f + 0.05f);

		if (rocks[i].getY() > 2.0f + 0.1f)
			rocks[i].setY(-2.0f - 0.05f);
		else if (rocks[i].getY() < -2.0f - 0.1f)
			rocks[i].setY(2.0f + 0.05f);
	}
}

void Application::render() {
	SDL_GL_SwapWindow(displayWindow);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	ship.ssDraw();
	for (int i = 0; i < ROCKS; ++i) {
		rocks[i].ssDraw();
	}
}

bool Application::checkCollision(Entity* entity1, Entity* entity2){
	if (entity1 == entity2){
		return false;
	}
	entity1->buildMatrix();
	entity2->buildMatrix();

	Matrix entity1Inverse = entity1->mat.inverse();
	Matrix entity2Inverse = entity2->mat.inverse();

	Vector ent2TL = Vector(-entity2->getWidth(), entity2->getHeight(), 0.0);
	Vector ent2BL = Vector(-entity2->getWidth(), -entity2->getHeight(), 0.0);
	Vector ent2TR = Vector(entity2->getWidth(), entity2->getHeight(), 0.0);
	Vector ent2BR = Vector(entity2->getWidth(), -entity2->getHeight(), 0.0);

	//transform to world coordinates
	ent2TL = entity2->mat * ent2TL;
	ent2BL = entity2->mat * ent2BL;
	ent2TR = entity2->mat * ent2TR;
	ent2BR = entity2->mat * ent2BR;

	// transform to entity 1 obj space
	ent2TL = entity1Inverse * ent2TL;
	ent2BL = entity1Inverse * ent2BL;
	ent2TR = entity1Inverse * ent2TR;
	ent2BR = entity1Inverse * ent2BR;

	//check x axis
	float minX = min(min(min(ent2TL.x, ent2BL.x), ent2TR.x), ent2BR.x);
	float maxX = max(max(max(ent2TL.x, ent2BL.x), ent2TR.x), ent2BR.x);

	if (!(minX <= entity1->getWidth() && maxX >= -entity1->getWidth())){
		return false;
	}

	//check y axis
	float minY = min(min(min(ent2TL.y, ent2BL.y), ent2TR.y), ent2BR.y);
	float maxY = max(max(max(ent2TL.y, ent2BL.y), ent2TR.y), ent2BR.y);

	if (!(minY <= entity1->getHeight() && maxY >= -entity1->getHeight())){
		return false;
	}

	return true;
}

void Application::collisionResponse(Entity* en1, Entity* en2) {
	Vector e1tl = Vector(-en1->getWidth(), en1->getHeight(), 0.0f);
	Vector e1tr = Vector(en1->getWidth(), en1->getHeight(), 0.0f);
	Vector e1bl = Vector(-en1->getWidth(), -en1->getHeight(), 0.0f);
	Vector e1br = Vector(en1->getWidth(), -en1->getHeight(), 0.0f);

	Vector e2tl = Vector(-en2->getWidth(), en2->getHeight(), 0.0f);
	Vector e2tr = Vector(en2->getWidth(), en2->getHeight(), 0.0f);
	Vector e2bl = Vector(-en2->getWidth(), -en2->getHeight(), 0.0f);
	Vector e2br = Vector(en2->getWidth(), -en2->getHeight(), 0.0f);

	e1tl = en1->mat * e1tl;
	e1bl = en1->mat * e1bl;
	e1tr = en1->mat * e1tr;
	e1br = en1->mat * e1br;
			
	e2tl = en2->mat * e2tl;
	e2bl = en2->mat * e2bl;
	e2tr = en2->mat * e2tr;
	e2br = en2->mat * e2br;

	float e1x = (e1tl.x + e1bl.x + e1tr.x + e1br.x) / 4.0f;
	float e2x = (e2tl.x + e2bl.x + e2tr.x + e2br.x) / 4.0f;
	float xPen = fabs(e1x - e2x);

	float e1y = (e1tl.y + e1bl.y + e1tr.y + e1br.y) / 4.0f;
	float e2y = (e2tl.y + e2bl.y + e2tr.y + e2br.y) / 4.0f;
	float yPen = fabs(e1y - e2y);

	float dist = sqrt(xPen * xPen + yPen * yPen);
	
	float e1moveX, e2moveX, e1moveY, e2moveY;

	if (e1x > e2x) {
		e1moveX = xPen / dist;
		e2moveX = -xPen / dist;
	}

	else {
		e1moveX = -xPen / dist;
		e2moveX = xPen / dist;
	}

	if (e1y > e2y) {
		e1moveY = yPen / dist;
		e2moveY = -yPen / dist;
	}

	else {
		e1moveY = -yPen / dist;
		e2moveY = yPen / dist;
	}

	en1->setXVel((en1->getXVel() * FIXED_TIMESTEP + e1moveX * FIXED_TIMESTEP * 3.5f));
	en1->setYVel((en1->getYVel() * FIXED_TIMESTEP + e1moveY * FIXED_TIMESTEP * 3.5f));
	en2->setXVel((en2->getXVel() * FIXED_TIMESTEP + e2moveX * FIXED_TIMESTEP * 3.5f));
	en2->setYVel((en2->getYVel() * FIXED_TIMESTEP + e2moveY * FIXED_TIMESTEP * 3.5f));
}