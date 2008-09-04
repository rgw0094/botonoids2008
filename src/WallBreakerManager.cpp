#include "WallBreakerManager.h"

extern HGE *hge;
extern hgeResourceManager *resources;
extern Grid *grid;
extern float gameTime;
extern hgeSprite *particleSprites[16];

/**
 * Constructor.
 */
WallBreakerManager::WallBreakerManager() {

}

/**
 * Destructor.
 */
WallBreakerManager::~WallBreakerManager() {
	reset();
}

/**
 * Draw all managed wall breakers.
 */
void WallBreakerManager::draw(float dt) {

	std::list<WallBreaker>::iterator i;
	for (i = wallBreakerList.begin(); i != wallBreakerList.end(); i++) {
		resources->GetSprite("wallbreaker")->Render(i->x, i->y);
	}

}

/**
 * Update all managed wall breakers.
 */
void WallBreakerManager::update(float dt) {

	//Loop through each item
	std::list<WallBreaker>::iterator i;
	for (i = wallBreakerList.begin(); i != wallBreakerList.end(); i++) {
		
		//Explode after a couple seconds
		if (gameTime > i->timeCreated + 2.0) {
			createExplosionAt(i->x + 14.0, i->y + 14.0);
			grid->breakWallAt(i->gridX, i->gridY);
			i = wallBreakerList.erase(i);
		}

	}

}

/**
 * Creates a new wall breaker to manage.
 */
void WallBreakerManager::addWallBreaker(int player, int x, int y) {
	
	//Create a new wall breaker
	WallBreaker newWallBreaker;
	newWallBreaker.player = player;
	newWallBreaker.gridX = x;
	newWallBreaker.gridY = y;
	newWallBreaker.x = grid->xOffset + (float)x * 33.0 + 2.0;
	newWallBreaker.y = grid->yOffset + (float)y * 33.0 + 2.0;
	newWallBreaker.timeCreated = gameTime;

	//Add it to the list
	wallBreakerList.push_back(newWallBreaker);
}

/**
 * Deletes all wallbreakers
 */
void WallBreakerManager::reset() {
	std::list<WallBreaker>::iterator i;
	for (i = wallBreakerList.begin(); i != wallBreakerList.end(); i++) {
		i = wallBreakerList.erase(i);
	}
}