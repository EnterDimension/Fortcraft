#include "base.h"

#include <vector>
#include <iostream>

#ifndef PLAYER_H
#define PLAYER_H

class player:base {
	SDL_Rect bounds;
	SDL_Surface* playerSprite;
	SDL_Rect playerSpriteClips[6];
	double frame;
	int velocityX, velocityY, health;
	bool onGround, jumping, moving;
	char direction;
	public:
		player(SDL_Surface* sprite);
		~player();
		SDL_Rect* getPlayerBounds();
		void setVelocityX(int velocity);
		int getVelocityX();
		void move(const std::vector<std::vector<int> >&world);
		void show(SDL_Surface* screen);
		void setJumping();
		void setMoving(bool b);
		void setDirection(char d);
		int getHealth();
		void setHealth(int);
};

#endif