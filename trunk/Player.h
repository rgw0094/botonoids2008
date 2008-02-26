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
#define GHOST_DURATION 25.0

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
	void drawItemWheel(float dt);
	void update(float dt);
	void updateItemSlots(float dt);
	void doColorChanging();
	void startFoundationMode(int numFoundations);
	void doMovement(float dt);
	void doStats(float dt);
	bool addItem(int item);
	void useItem(float dt);
	int itemInSlot(int slot);
	int getItemQuantityPerToken(int item);
	int numEmptyItemSlots();
	void dealDamage(float damage);
	void slowPlayer();
	void die();
	void updatePunchingGlove(float dt);
	int findClosestEnemy();

	//Variables
	int gridX, gridY, lastGridX, lastGridY;
	float x,y;
	int score;
	int whichBotonoid;		//Which botonoid character
	int playerNum;
	int facing;
	int movingDirection;
	int numChangesLeft;		//Number of changes left in color change mode
	int numWallsLeft;
	int health;
	float positionAngles[4];	//angles corresponding to position in item wheel
	float itemWheelX, itemWheelY;
	bool collisionAt(int x, int y);

	float punchingGloveAngle;
	int punchingGloveTarget;

	//Time variables
	float startedMoving;
	float endedColorChange;
	float timeSlowed;
	float timePunched;
	float timeGhosted;

	//State variables
	bool colorChangeMode;
	bool foundationMode;
	bool buildWallPressed;
	bool lockedOnByMissile;
	bool slowed;
	bool punching;
	bool ghostMode;
	bool dead;

	//Objects
	hgeRect *collisionBox;
	hgeSprite *emptyItemSlot;
	hgeParticleSystem *slowEffectParticle;
	hgeAnimation *punchingGlove;
	ItemSlot itemSlots[4];

};

#endif