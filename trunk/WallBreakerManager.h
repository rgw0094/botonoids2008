#ifndef _WALLBREAKERMANAGER_H_
#define _WALLBREAKERMANAGER_H_

#include "game.h"

struct WallBreaker {
	int player,gridX, gridY;
	float x, y;
	float timeCreated;
};

class WallBreakerManager {

public:

	WallBreakerManager();
	~WallBreakerManager();

	//methods
	void update(float dt);
	void draw(float dt);
	void addWallBreaker(int player, int x, int y);
	void reset();

	//Variables
	std::list<WallBreaker> wallBreakerList;

};

#endif