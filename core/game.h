#include "base.h"
#include "player.h"
#include "projectile.h"
#include "entity_hostile.h"

#include <vector>
#include <iostream>
#include <fstream>

#ifndef GAME_H
#define GAME_H

class game:public base {
	SDL_Surface *screen, *background, *worldTextures, *arrowSprite, *slimeSprite;
	SDL_Rect veiw, relative;
	std::vector<std::vector<int> > world;
	std::vector<projectile*> projectiles;
	std::vector<entity_hostile*> hostileEntities;
	bool direction[2];
	int fpscap;
	bool running;
	player* localPlayer;

	SDL_Surface* loadImage(const char* filename);
	void loadWorld(const char* filename);
	void showWorld();
	void handleEvents();

	public:
		static const int SCREEN_WIDTH = 640;
		static const int SCREEN_HEIGHT = 480;

		game();
		~game();
		void start();
};

#endif