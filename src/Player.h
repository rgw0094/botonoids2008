#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "game.h"

#define EMPTY -1
#define TOP_SLOT 0
#define RIGHT_SLOT 1
#define BOTTOM_SLOT 2
#define LEFT_SLOT 3
#define PI 3.1415926f
#define SPIN_TIME .2f

#define SPEED 150.0
#define SLOWED_SPEED 75.0
#define SLOW_DURATION 10.0
#define PUNCH_DELAY 0.4

struct ItemSlot {
	int code;
	int quantity;
	int position;
	float angle;
	float targetAngle;
	float x,y;
};

class Player {

public:

	Player(int _x, int _y, int _playerNum, int _whichBotonoid);
	~Player();

	//methods
	void draw(float dt);
	void update(float dt);
	bool addItem(int item);
	bool dealDamage(float damage);
	void slowPlayer();
	void startFoundationMode(int numFoundations);
	bool isDead();
	float getTimeUntilRespawn();
	bool testCollision(hgeRect *box);
	void hitWithPunchingGlove();

	int gridX, gridY, startGridX, startGridY;
	float x,y;
	int score;
	int whichBotonoid;		//Which botonoid character
	int playerNum;
	int facing;
	int movingDirection;
	int numChangesLeft;		//Number of changes left in color change mode
	int numWallsLeft;
	int health;
	

private:

	void drawItemWheel(float dt);
	bool collisionAt(int x, int y);
	void updateItemSlots(float dt);
	void doColorChange();
	void doMovement(float dt);
	void doStats(float dt);
	void useItem(float dt);
	int itemInSlot(int slot);
	int getItemQuantityPerToken(int item);
	int numEmptyItemSlots();
	void die();
	void updatePunchingGlove(float dt);
	int findClosestEnemy();
	float punchGloveDelay;

	float positionAngles[4];	//angles corresponding to position in item wheel
	float itemWheelX, itemWheelY;
	float punchingGloveAngle;
	int punchingGloveTarget;

	//Time variables
	float startedMoving;
	float endedColorChange;
	float timeSlowed;
	float timePunched;
	float timeGhosted;
	float timeKilled;
	float timeStartedFlashing;
	float timeStartedStun;

	//State variables
	bool moving;
	bool colorChangeMode;
	bool foundationMode;
	bool buildWallPressed;
	bool lockedOnByMissile;
	bool slowed;
	bool punching;
	bool ghostMode;
	bool dead;
	bool flashing;
	bool stunned;

	hgeRect *collisionBox;
	hgeRect *punchingGloveBox;
	hgeSprite *emptyItemSlot;
	hgeParticleSystem *slowEffectParticle;
	hgeAnimation *punchingGlove;
	ItemSlot itemSlots[4];

};

#endif