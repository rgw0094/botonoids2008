#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "game.h"

#define EMPTY -1
#define RIGHT_SLOT 0
#define BOTTOM_SLOT 1
#define LEFT_SLOT 2
#define TOP_SLOT 3
#define PI 3.1415926f

struct ItemSlot {
	int code;
	int quantity;
	int position;
	float angle;
	float targetAngle;
};

class Player {

public:

	Player(int _x, int _y, int _playerNum, int _whichBotonoid);
	~Player();

	//methods
	void draw(float dt);
	void update(float dt);
	void updateItemSlots(float dt);
	void doColorChanging();
	void startFoundationMode(int numFoundations);
	void doMovement(float dt);
	void doStats(float dt);
	void addItem(int item);

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

	//Time variables
	float startedMoving;
	float timeToMove;
	float endedColorChange;
	float timeChangedItem;

	//State variables
	bool colorChangeMode;
	bool foundationMode;
	bool buildWallPressed;

	//Objects
	hgeRect *collisionBox;
	ItemSlot itemSlots[4];

};

#endif