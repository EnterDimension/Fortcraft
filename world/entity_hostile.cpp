#include "entity_hostile.h"

entity_hostile::entity_hostile(SDL_Surface* entitySprite, int boundsX, int boundsY, int velX, int velY){
	sprite = entitySprite;
	bounds = { boundsX, boundsY, sprite->w / 2, sprite->h };
	velocityX = velX;
	velocityY = velY;
	onGround = 0;
	frame = 0.0;
	for (int i = 0; i < 2; i++){
		entitySpriteClips[i] = { i * 40, 0, 40, 40 };
	}
	health = 100;
}
void entity_hostile::move(const std::vector<std::vector<int> >&world){
	int start = (base::coord.x - (base::coord.x % base::TILE_SIZE)) / base::TILE_SIZE - 2;
	int end = (base::coord.x + base::coord.w + (base::TILE_SIZE - (base::coord.x + base::coord.w) % base::TILE_SIZE)) / base::TILE_SIZE + 2;
	if (start < 0)
		start = 0;
	if (end > world[0].size())
		end = world[0].size();
	bool collision = 0;
	for (size_t h = 0; h < world.size(); ++h) {
		for (size_t w = 0; w < end; ++w) {
			if (world[h][w] == 0)
				continue;
			SDL_Rect entityTo = { w * 50, h * 50, 50, 50 };
			if (collides(&bounds, &entityTo)){
				if (entityTo.y >= bounds.y + bounds.h - 11){
					onGround = 1;
					velocityY = 0;
					collision = 1;
				}
				else if (entityTo.y + entityTo.h <= bounds.y + 11){
					velocityX = -velocityX;
				}
				if (bounds.x + bounds.w >= entityTo.x - 5 && bounds.y + bounds.h >= entityTo.y + 6 && bounds.x + bounds.w <= entityTo.x + 20){
					velocityX = -velocityX;
				}
			}
		}
	}
	if (!collision) 
		velocityY = 5;
	bounds.x += velocityX;
	bounds.y += velocityY;
	if (velocityY == 0) {
		frame += 0.2;
		if (frame >= 1.4)
			frame = 0.0;
		else if (frame >= 3.4)
			frame = 1.5;
	}
}
void entity_hostile::show(SDL_Surface* screen){
	SDL_Rect relative = { bounds.x - coord.x, bounds.y, 40, 40 };
	SDL_BlitSurface(sprite,&entitySpriteClips[0], screen, &relative);
}
SDL_Rect* entity_hostile::getBounds(){
	return &bounds;
}
int entity_hostile::getHealth() {
	return health;
}
void entity_hostile::setHealth(int h){
	health = h;
}