#include "generic_functions.h"

GLuint LoadTexture(const char *image_path, GLuint imageFormat) {
	SDL_Surface *surface = IMG_Load(image_path);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	SDL_FreeSurface(surface);
	return textureID;
}

float lerp(float v0, float v1, float t) {
	return (1.0 - t)*v0 + t*v1;
}

float randomNumber(float min, float max) { //got this concept online with a small modification
	float randNum = ((float)rand()) / (float)RAND_MAX;
	float range = max - min;
	//while ((randNum * range) + min < 0.5 && (randNum * range) + min > -0.5) {
	//	randNum = ((float)rand()) / (float)RAND_MAX;
	//}
	return (randNum * range) + min;
}