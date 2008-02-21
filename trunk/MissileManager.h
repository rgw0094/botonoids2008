#ifndef _MISSILEMANAGER_H_
#define _MISSILEMANAGER_H_

#include "game.h"

#define MISSILE_START_SPEED 300.0
#define MISSILE_TURN_SPEED 1.2

struct Missile {
	hgeRect *collisionBox;
	hgeParticleSystem *trail;
	int player, x, y, target;
	float dx, dy;
	float angle;
	float timeExploded;
};

class MissileManager {

public:

	MissileManager();
	~MissileManager();

	//methods
	void update(float dt);
	void draw(float dt);
	void addMissile(int player, int x, int y);
	void explodeAt(float x, float y);
	void reset();

	//Objects
	std::list<Missile> missileList;


};

#endif