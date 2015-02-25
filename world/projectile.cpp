#include "projectile.h"

projectile::projectile(SDL_Surface* projectileSprite, int boundsX, int boundsY, int velX, int velY){
	sprite = projectileSprite; 
	bounds = { boundsX, boundsY, sprite->w, sprite->h };
	velocityX = velX;
	velocityY = velY;
}
SDL_Rect* projectile::getBounds(){
	return &bounds;
}
void projectile::move(){
	bounds.x += velocityX;
	bounds.y += velocityY;
}
void projectile::show(SDL_Surface* screen){
	SDL_BlitSurface(sprite, NULL, screen, &bounds);
}