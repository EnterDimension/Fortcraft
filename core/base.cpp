#include "base.h"

bool base::collides(SDL_Rect* collider, SDL_Rect* object){
	if (collider->y >= object->y + object->h)        
		return 0;       
	if (collider->x >= object->x + object->w) 
		return 0;
	if (collider->y + collider->h <= object->y)       
		return 0;
	if (collider->x + collider->w <= object->x)        
		return 0;
	return 1;       
}