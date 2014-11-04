#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <fstream>
#include <ctime>
#include <algorithm>
#include <random>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "generic_functions.h"
#include "entity.h"
#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
#define spriteCountX 16
#define spriteCountY 8
#define TILE_SIZE 0.15f
#define ROCKS 10
#define MAX_BULLETS 25
#define WORLD_OFFSET_X 0
#define WORLD_OFFSET_Y 0
#define RANDOM_DIRECTION (direction::directions)rand()%2

class Application {
public:
	Application();
	~Application();

	bool checkCollision(Entity*, Entity*);
	void collisionResponse(Entity*, Entity*);

	bool UpdateAndRender();
private:
	SDL_Event event;
	SDL_Window* displayWindow;

	GLuint ssID;
	GLuint pokemonSS;

	Entity ship;
	Entity rocks[ROCKS];
	Entity bullets[MAX_BULLETS];

	const Uint8 *keys;

	unsigned char **levelData;
	int mapWidth;
	int mapHeight;
	int enemyIndex;
	int playerFacing;
	int bulletIndex;
	float shootCD;

	float lastFrameTicks;
	float timeLeftOver;

	void Init();
	void update(float);
	void FixedUpdate();
	void render();
};