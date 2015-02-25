#include "base.h"

#include <vector>

#ifndef ENTIY_HOSTILE_H
#define ENTITY_HOSTILE_H

class entity_hostile:base {
	SDL_Rect bounds;
	int velocityX, velocityY, health;
	SDL_Surface* sprite;
	SDL_Rect entitySpriteClips[2];
	double frame;
	bool onGround;
	public:
		entity_hostile(SDL_Surface* entitySprite, int boundsX, int boundsY, int velocityX, int velocityY);
		void move(const std::vector<std::vector<int> >&world);
		void show(SDL_Surface* screen);
		SDL_Rect* getBounds();
		int getHealth();
		void setHealth(int);
};

#endif