#include "game.h"


SDL_Rect base::coord;

game::game(){
	SDL_Init(SDL_INIT_EVERYTHING);
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE);
	worldTextures = loadImage("textures/world/textures.bmp"); 
	arrowSprite = loadImage("textures/entities/projectiles/arrow.bmp");
	background = loadImage("textures/world/bg.bmp");	
	slimeSprite = loadImage("textures/entities/slime.bmp");
	base::coord.x = base::coord.y = veiw.x = veiw.y = 0;
	base::coord.w = veiw.w = SCREEN_WIDTH;
	base::coord.h = veiw.h = SCREEN_HEIGHT;
	direction[0] = direction[1] = 0;
	fpscap = 30;
	running = true;
	localPlayer = new player(loadImage("textures/playerSprite.bmp"));
	hostileEntities.push_back(new entity_hostile(slimeSprite,30,40,1,0)); 
}
game::~game() {
	SDL_FreeSurface(worldTextures);
	SDL_FreeSurface(background);
	SDL_FreeSurface(arrowSprite);
	for (size_t i = 0; i < projectiles.size(); ++i)
		delete projectiles[i];
	for (size_t i = 0; i < hostileEntities.size(); ++i)
		delete hostileEntities[i];
	SDL_Quit();
}
SDL_Surface* game::loadImage(const char* filename){
	std::string resourceLocation = "cl/resources/";
	SDL_Surface* image = SDL_DisplayFormat(SDL_LoadBMP(resourceLocation.append(filename).c_str()));
	SDL_SetColorKey(image, SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 0xff, 0x00, 0xff));
	return image;
}
void game::handleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)){
		switch (event.type){
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym){
					case SDLK_a:
						direction[0] = 1;
						localPlayer->setMoving(1);
						localPlayer->setDirection('l');
						break;
					case SDLK_d:
						direction[1] = 1;
						localPlayer->setMoving(1);
						localPlayer->setDirection('r');
						break;
					case SDLK_SPACE:
						localPlayer->setJumping();
						break;
					case SDLK_f:
						projectiles.push_back(new projectile(arrowSprite, localPlayer->getPlayerBounds()->x + localPlayer->getPlayerBounds()->w, localPlayer->getPlayerBounds()->y + 15,5,0));
						break;
				}
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.sym){
					case SDLK_a:
						direction[0] = 0;
						localPlayer->setMoving(0);
						break;
					case SDLK_d:
						direction[1] = 0;
						localPlayer->setMoving(0);
						break;
				}
				break;
		}

	}
}
void game::loadWorld(const char* filename){
	std::ifstream in(filename);
	if (!in.is_open()) {
		std::cout << "Error: Failed at loading map file" << std::endl;
		return;
	}
	int width, height, current;
	in >> width;
	in >> height;
	for (int h = 0; h < height; h++) {
		std::vector<int> vec;
		for (int w = 0; w < width; w++) {
			if (in.eof()){
				std::cout << "Error: File end reached too soon" << std::endl;
				return;
			}
			in >> current;
			if (current >= 0 && current <= 4){
				vec.push_back(current);
			}
			else {
				vec.push_back(0);
			}
		}
		world.push_back(vec);
	}
	in.close();
}
void game::showWorld() {
	int start = (base::coord.x - (base::coord.x % base::TILE_SIZE)) / base::TILE_SIZE;
	int end = (base::coord.x + base::coord.w + (base::TILE_SIZE - (base::coord.x + base::coord.w) % base::TILE_SIZE)) / TILE_SIZE;
	if (start < 0) start = 0;
	if (end > world[0].size()) end = world[0].size();
	for (size_t h = 0; h < world.size(); ++h) {
		for (size_t w = 0; w < end; ++w) {
			if (world[h][w] != 0) {
				SDL_Rect tileTexture = { (world[h][w] - 1) * base::TILE_SIZE, 0, base::TILE_SIZE, base::TILE_SIZE };
				SDL_Rect relative = { w * base::TILE_SIZE - base::coord.x, h * 50 };
				SDL_BlitSurface(worldTextures, &tileTexture, screen, &relative);
			}
		}
	}
}

void game::start(){
	loadWorld("cl/user/saves/1.map");
	while (running){
		handleEvents();

		if (direction[0]) {
			if (localPlayer->getPlayerBounds()->x > SCREEN_WIDTH / 2) 
				localPlayer->setVelocityX(-1);
			else {
				localPlayer->setVelocityX(0);
				veiw.x--;
				base::coord.x--;
			}
			if (veiw.x < 0) veiw.x = 2000 - SCREEN_WIDTH;
		} 
		else if (direction[1]){
			if (localPlayer->getPlayerBounds()->x < SCREEN_WIDTH / 2)
				localPlayer->setVelocityX(1);
			else {
				localPlayer->setVelocityX(0);
				veiw.x++;
				base::coord.x++;
			}
			if (veiw.x >= 2000 - SCREEN_WIDTH) 
				veiw.x = 0;
		}
		else {
			localPlayer->setVelocityX(0);
		}
		int startPos = (base::coord.x - (base::coord.x % base::TILE_SIZE)) / base::TILE_SIZE;
		int endPos = (base::coord.x + base::coord.w + (base::TILE_SIZE - (base::coord.x + base::coord.w) % base::TILE_SIZE)) / 50;
		if (startPos < 0)
			startPos = 0;
		if (endPos  >world[0].size())
			endPos = world[0].size();
		for (int h = 0; h < world.size(); h++)
			for (int w = startPos; w < endPos; w++) {
				if (world[h][w] == 0)
					continue;
				SDL_Rect projectileTo = { w * 50 - base::coord.x, h * 50, 50, 50 };
				for (int p = 0; p < projectiles.size(); p++)      
					if (collides(projectiles[p]->getBounds(), &projectileTo))  {
						delete projectiles[p];     
						projectiles.erase(projectiles.begin() + p);       
					}
			}
		for (int i = 0; i < projectiles.size(); i++)     
			if (projectiles[i]->getBounds()->x >= screen->w || projectiles[i]->getBounds()->x <= 0){
				delete projectiles[i];
				projectiles.erase(projectiles.begin() + i);
			}
		for (int i = 0; i < projectiles.size(); i++)
			for (int e = 0; e < hostileEntities.size(); e++){
				SDL_Rect relative = { hostileEntities[e]->getBounds()->x - base::coord.x, hostileEntities[e]->getBounds()->y, 40, 40 };
				if (collides(&relative, projectiles[i]->getBounds())){
					hostileEntities[e]->setHealth(hostileEntities[e]->getHealth() - 20);
					delete projectiles[i];
					projectiles.erase(projectiles.begin() + i);
				}
			}
		for(int i = 0; i < hostileEntities.size(); i++){
			if (hostileEntities[i]->getHealth() <= 0){
				delete hostileEntities[i];
				hostileEntities.erase(hostileEntities.begin() + i);
			}
		}
		for(int j = 0; j<hostileEntities.size(); j++)      
		{
			SDL_Rect tmprect = { hostileEntities[j]->getBounds()->x - base::coord.x, hostileEntities[j]->getBounds()->y, 40, 40 }; 
			if (collides(&base::coord, hostileEntities[j]->getBounds())) 
			{
				if (collides(&tmprect, localPlayer->getPlayerBounds()))     
				{
					if (localPlayer->getPlayerBounds()->y + localPlayer->getPlayerBounds()->h >= hostileEntities[j]->getBounds()->y && localPlayer->getPlayerBounds()->y + localPlayer->getPlayerBounds()->h <= hostileEntities[j]->getBounds()->y + 10)
					{
						localPlayer->setJumping();
					}
					else{
						localPlayer->setHealth(localPlayer->getHealth() - 5); 
					}
				}
				hostileEntities[j]->move(world); 
			}
		}

		localPlayer->move(world);
		for (size_t i = 0; i < projectiles.size(); ++i)
			projectiles[i]->move();
		for (size_t i = 0; i < hostileEntities.size(); ++i)
			hostileEntities[i]->move(world);

		SDL_BlitSurface(background, &veiw, screen, NULL);
		showWorld();
		localPlayer->show(screen);
		for (size_t i = 0; i < projectiles.size(); ++i)
			projectiles[i]->show(screen);
		for (size_t i = 0; i < hostileEntities.size(); ++i)
			hostileEntities[i]->show(screen);
		SDL_Flip(screen);
		
		if (localPlayer->getHealth() <= 0) {
			std::cout << "game over" << std::endl;
			running = false;
		}
			
		//secure frames at frame cap (or close enough)
		Uint32 fps = SDL_GetTicks();
		if (1000 / fpscap > SDL_GetTicks() - fps)
			SDL_Delay(1000 / fpscap - (SDL_GetTicks() - fps));
	}
}