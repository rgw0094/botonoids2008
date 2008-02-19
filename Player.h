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

	//Variables
	int gridX, gridY, lastGridX, lastGridY;
	float x,y;
	float speed;
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

	//Time variables
	float startedMoving;
	float timeToMove;
	float endedColorChange;

	//State variables
	bool colorChangeMode;
	bool foundationMode;
	bool buildWallPressed;
	bool lockedOnByMissile;

	//Objects
	hgeRect *collisionBox;
	hgeSprite *emptyItemSlot;
	ItemSlot itemSlots[4];

};

#endif