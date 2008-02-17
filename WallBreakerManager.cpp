#include "WallBreakerManager.h"

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
	//reset();
}

/**
 * Draw all managed wall breakers.
 */
void WallBreakerManager::draw(float dt) {

	std::list<WallBreaker>::iterator i;
	for (i = wallBreakerList.begin(); i != wallBreakerList.end(); i++) {
		if (i->exploded) {
			i->explosion->Update(dt);
			i->explosion->Render();
		} else {
			resources->GetSprite("wallbreaker")->Render(i->x, i->y);
		}
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
		if (!i->exploded && gameTime > i->timeCreated + 2.0) {
			i->explosion = new hgeParticleSystem("Data/particle1.psi", particleSprites[0]);
			i->explosion->FireAt(i->x + 14.0, i->y + 14.0);
			i->exploded = true;
			grid->breakWallAt(i->gridX, i->gridY);
		}

		//After the explosion is done, delete the wallbreaker
		if (gameTime > i->timeCreated + 5.0) {
			delete i->explosion;
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
	newWallBreaker.exploded = false;

	//Add it to the list
	wallBreakerList.push_back(newWallBreaker);
}

/**
 * Deletes all wallbreakers
 */
void WallBreakerManager::reset() {
	std::list<WallBreaker>::iterator i;
	for (i = wallBreakerList.begin(); i != wallBreakerList.end(); i++) {
		if (i->explosion) delete i->explosion;
		i = wallBreakerList.erase(i);
	}
}