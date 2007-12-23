#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "game.h"

class Player {

public:

	Player(int _x, int _y, int _playerNum, int _whichBotonoid);
	~Player();

	//methods
	void draw(float dt);
	void update(float dt);
	void doColorChanging();
	void startFoundationMode(int numFoundations);
	void doMovement(float dt);
	void doStats(float dt);

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

	//Time variables
	float startedMoving;
	float timeToMove;
	float endedColorChange;

	//State variables
	bool colorChangeMode;
	bool foundationMode;
	bool buildWallPressed;

	//Objects
	hgeRect *collisionBox;

};

#endif