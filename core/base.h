#include <SDL.h>

#ifndef BASE_H
#define BASE_H

class base {
	protected:
		bool collides(SDL_Rect* collider, SDL_Rect* object);
	public:
		static SDL_Rect coord;
		static const int TILE_SIZE = 50;
};

#endif