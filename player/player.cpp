#include "player.h"
#include "game.h"

player::player(SDL_Surface* sprite){
	bounds = { game::SCREEN_WIDTH / 2, 0, 50, 100 };
	playerSprite = sprite;
	for (int i = 0; i < 6; i++){
		playerSpriteClips[i] = { i * 30, 0, 30, 50 };
	}
	velocityX, velocityY, onGround, jumping, moving = 0;
	direction = 'r';
	frame = 0.0;
	health = 100;

}
player::~player(){
	SDL_FreeSurface(playerSprite);
}
SDL_Rect* player::getPlayerBounds(){
	return &bounds;
}
void player::setVelocityX(int velocity){
	velocityX = velocity;
}
int player::getVelocityX(){
	return velocityX;
}
void player::move(const std::vector<std::vector<int> >&world){
	int start = (base::coord.x - (base::coord.x % base::TILE_SIZE)) / base::TILE_SIZE;
	int end = (base::coord.x + base::coord.w + (base::TILE_SIZE - (base::coord.x + base::coord.w) % base::TILE_SIZE)) / TILE_SIZE;
	if (start < 0) 
		start = 0;
	if (end > world[0].size()) 
		end = world[0].size();
	bool collision = 0;
	for (size_t h = 0; h < world.size(); ++h) {
		for (size_t w = 0; w < end; ++w) {
			if (world[h][w] == 0)
				continue;
			SDL_Rect playerTo = { w * 50 - base::coord.x, h * 50, 50, 50 };
			if (collides(&bounds, &playerTo)){
				if (playerTo.y >= bounds.y + bounds.h - 11){
					onGround = 1;
					velocityY = 0;
					collision = 1;
				}
				else if (playerTo.y + playerTo.h <= bounds.y + 11){
					bounds.y++;
					velocityY = 5;
				}
				if (bounds.x + bounds.w >= playerTo.x - 5 && bounds.y + bounds.h >= playerTo.y + 6 && bounds.x + bounds.w <= playerTo.x + 20){
					velocityX = 0;
					bounds.x--;
				}
				else if (bounds.x <= playerTo.x + playerTo.w && bounds.y + bounds.h >= playerTo.y + 6) {
					velocityX = 0;
					bounds.x++;
				}
			}
		}
	}
	if (!collision && !jumping) {    
		velocityY = 5; 
	}
	if (jumping && velocityY < 5) {
		velocityY++;
	}
	else
		jumping = 0;
	bounds.x += velocityX;
	bounds.y += velocityY;
	if (moving) {
		if (velocityY == 0) {
			frame += 0.2;
			if (direction == 'r' && frame >= 1.4)
				frame = 0.0;
			else if (direction == 'l' && frame >= 3.4)
				frame = 1.5;
		}
		else if (velocityY < 5) {
			if (direction == 'r')
				frame = 4.0;
			else if (direction == 'l')
				frame = 5.0;
		}
	}
}
void player::setJumping(){
	if (onGround && !jumping) {
		jumping = 1;
		onGround = 0;
		velocityY = -17;
		bounds.y -= 5;
	}
}
void player::setMoving(bool b){
	moving = b;
}
void player::setDirection(char d){
	if (d == 'r' || d == 'l' && direction != d){
		direction = d;
		if (direction == 'r')
			frame = 0.0;
		else
			frame = 1.6;
	}
		
}
int player::getHealth() {
	return health;
}
void player::setHealth(int h){
	health = h;
}
void player::show(SDL_Surface* screen){
	SDL_BlitSurface(playerSprite, &playerSpriteClips[(int)(frame + 0.5)], screen, &bounds);
}