#include "MissileManager.h"

extern HGE *hge;
extern hgeResourceManager *resources;
extern Grid *grid;
extern Player *players[3];
extern GameInfo gameInfo;
extern hgeParticleManager *explosionManager;
extern float gameTime;

MissileManager::MissileManager() {

}

MissileManager::~MissileManager() {
	reset();
}

/**
 * Updates all managed missiles
 */
void MissileManager::update(float dt) {
	std::list<Missile>::iterator i;
	for (i = missileList.begin(); i != missileList.end(); i++) {

		//Skip this missile if it exploded
		if (i->timeExploded < 0.0) {

			//Find angle to seek target
			float xDist = i->x - players[i->target]->x;
			float targetAngle = atan((i->y - players[i->target]->y) / xDist);

			//----------Do a bunch of SHIT----------
			while (i->angle < 0.0) i->angle += 2.0*PI;
			while (i->angle > 2.0*PI ) i->angle -= 2.0*PI;
			while (targetAngle < 0.0) targetAngle += 2.0*PI;
			while (targetAngle > 2.0*PI ) targetAngle -= 2.0*PI;
			float temp = i->angle - targetAngle;
			while (temp < 0) temp += 2.0*PI;
			while (temp > 2.0*PI) temp -= 2.0*PI;
			if (xDist > 0) {
				if (temp <= PI) {
					i->angle += MISSILE_TURN_SPEED * dt;
				} else if (temp > PI) {
					i->angle -= MISSILE_TURN_SPEED * dt;
				}
			} else {
				if (temp <= PI) {
					i->angle -= MISSILE_TURN_SPEED * dt;
				} else if (temp > PI) {
					i->angle += MISSILE_TURN_SPEED * dt;
				}
			}
			//----------------------------------------

			//Update x and y velocities based on angle
			i->dx = cos(i->angle) * MISSILE_START_SPEED;
			i->dy = sin(i->angle) * MISSILE_START_SPEED;

			//Update position
			i->x += i->dx * dt;
			i->y += i->dy * dt;

			//Check for collision with players!!!
			boolean exploded = false;
			int player = 0;
			i->collisionBox->SetRadius(i->x, i->y, 10.0);
			while (!exploded && player < gameInfo.numPlayers) {
				if (player != i->player && i->collisionBox->Intersect(players[player]->collisionBox)) {
					
					//Collided with player
					i->timeExploded = gameTime;
					exploded = true;
					players[player]->dealDamage(1.0);

				}
				player++;
			}

			//Check for offscreen - always do this last!!!!!
			if (!exploded && !isInBounds(i->x, i->y)) {
				i->timeExploded = gameTime;
				exploded = true;
			}

			if (exploded) createExplosionAt(i->x, i->y);

		}

		//Actually delete the missile after it explodes 
		//so the trail can finish
		if (i->timeExploded > 0 && gameTime > i->timeExploded + 0.4) {
			if (i->collisionBox) delete i->collisionBox;
			if (i->trail) delete i->trail;
			i = missileList.erase(i);
		}

	}
}

/**
 * Draws all managed missiles
 */
void MissileManager::draw(float dt) {
	std::list<Missile>::iterator i;
	for (i = missileList.begin(); i != missileList.end(); i++) {

		//Draw the missile trail
		i->trail->Update(dt);
		i->trail->MoveTo(i->x, i->y);
		i->trail->Render();

		//Draw the missile if it hasn't exploded yet
		if (i->timeExploded < 0.0) {
			resources->GetSprite("missile")->RenderEx(i->x, i->y, i->angle);
		}

		//Draw a crosshair on the missile's target
		resources->GetSprite("crosshair")->Render(players[i->target]->x, players[i->target]->y);

	}
}

/**
 * Creates a new missile to manage.
 */
void MissileManager::addMissile(int player, int x, int y) {
	
	//Create a new missile
	Missile newMissile;
	newMissile.player = player;
	newMissile.x = grid->xOffset + (float)x * 33.0 + 2.0;
	newMissile.y = grid->yOffset + (float)y * 33.0 + 2.0;
	newMissile.collisionBox = new hgeRect();
	newMissile.angle = 0.0;
	newMissile.trail = new hgeParticleSystem(&resources->GetParticleSystem("missileTrail")->info);
	newMissile.trail->FireAt(newMissile.x, newMissile.y);
	newMissile.timeExploded = -10.0;

	//Set initial position and velocity based on the player's direction
	if (players[player]->facing == LEFT) {
		newMissile.x -= 30;
		newMissile.angle = PI;
	} else if (players[player]->facing == RIGHT) {
		newMissile.x += 30;	
		newMissile.angle = 0.0;
	} else if (players[player]->facing == UP) {
		newMissile.y -= 30;
		newMissile.angle = 1.5 * PI;
	} else if (players[player]->facing == DOWN) {
		newMissile.y += 30;
		newMissile.angle = .5 * PI;
	}

	//Calculate starting velocities
	newMissile.dx = cos(newMissile.angle) * MISSILE_START_SPEED;
	newMissile.dy = sin(newMissile.angle) * MISSILE_START_SPEED;

	//Acquire a target the closest other player
	float shortestDist = 9999999.0;
	for (int i = 0; i < gameInfo.numPlayers; i++) {
		if (i != player) {
			float distance = dist(x, y, players[i]->gridX, players[i]->gridY);
			if (distance < shortestDist) {
				shortestDist = distance;
				newMissile.target = i;
			}
		}
	}

	//Add it to the list
	missileList.push_back(newMissile);
}

/**
 * Removes all managed missiles
 */
void MissileManager::reset() {
	std::list<Missile>::iterator i;
	for (i = missileList.begin(); i != missileList.end(); i++) {
		if (i->collisionBox) delete i->collisionBox;
		if (i->trail) delete i->trail;
		i = missileList.erase(i);
	}
}