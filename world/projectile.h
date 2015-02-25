#include "base.h"

#include <iostream>

#ifndef PROJECTILE_H
#define PROJECTILE_H

class projectile {
	SDL_Rect bounds;
	int velocityX, velocityY;
	SDL_Surface* sprite;

	public:
		projectile(SDL_Surface* projectileSprite, int boundsX, int boundsY, int velocityX, int velocityY);
		SDL_Rect* getBounds();
		void move();
		void show(SDL_Surface* screen);
};

#endif