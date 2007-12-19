#include "game.h"

extern HGE *hge;
extern Grid *grid;
extern GUI *gui;
extern hgeAnimation *botonoidGraphics[NUM_BOTONOIDS];
extern controls playerControls[3];
extern hgeResourceManager *resources;
extern hgeFont *debugFont;

/**
 * Constructor
 */
Player::Player(int _x, int _y, int _playerNum, int _whichBotonoid) {

	gridX = lastGridX = _x;
	gridY = lastGridY = _y;
	x = grid->xOffset + gridX*(GRID_SIZE+1) + (GRID_SIZE+1)/2;
	y = grid->yOffset + gridY*(GRID_SIZE+1) + (GRID_SIZE+1)/2;
	playerNum = _playerNum;
	whichBotonoid = _whichBotonoid;
	facing = DOWN;
	speed = 150.0f;
	score = 0;
	health = 3;

	colorChangeMode = foundationMode = false;

	startedMoving = -10.0f;
	endedColorChange = -10.0f;
	timeToMove = (GRID_SIZE+1) / speed;

}

/**
 * Destructor
 */
Player::~Player() {

}

void Player::update(float dt) {

	//Find current grid location
	gridX = (x - grid->xOffset) / (GRID_SIZE+1);
	gridY = (y - grid->yOffset) / (GRID_SIZE+1);

	//If the player just changed squares, doColorChanging() at the new square
	if ((gridX != lastGridX || gridY != lastGridY) && colorChangeMode) {
		doColorChanging();
	}

	//Update previous position
	lastGridX = gridX;
	lastGridY = gridY;

	//Update movement
	if (hge->Timer_GetTime() < startedMoving + timeToMove) {
		switch (movingDirection) {
			case UP: y -= speed*dt; break;
			case DOWN: y += speed*dt; break;
			case LEFT: x -= speed*dt; break;
			case RIGHT: x += speed*dt; break;
		}

	//Start movement
	} else {

		x = grid->xOffset + gridX*(GRID_SIZE+1) + (GRID_SIZE+1)/2;
		y = grid->yOffset + gridY*(GRID_SIZE+1) + (GRID_SIZE+1)/2;

		int oldDir = facing;

		//Left
		if (hge->Input_GetKeyState(playerControls[playerNum].moveLeft)) {
			facing = movingDirection = LEFT;
			if (gridX > 0 && !(grid->walls[gridX-1][gridY] != -1 && grid->walls[gridX-1][gridY] != playerNum)) {
				startedMoving = hge->Timer_GetTime();
			}
		//Right
		} else if (hge->Input_GetKeyState(playerControls[playerNum].moveRight)) {
			facing = movingDirection =  RIGHT;
			if (gridX < grid->width-1 && !(grid->walls[gridX+1][gridY] != -1 && grid->walls[gridX+1][gridY] != playerNum)) {
				startedMoving = hge->Timer_GetTime();
			}
		//Down
		} else if (hge->Input_GetKeyState(playerControls[playerNum].moveDown)) {
			facing = movingDirection =  DOWN;
			if (gridY < grid->height-1 && !(grid->walls[gridX][gridY+1] != -1 && grid->walls[gridX][gridY+1] != playerNum)) {
				startedMoving = hge->Timer_GetTime();
			}
		//Up
		} else if (hge->Input_GetKeyState(playerControls[playerNum].moveUp)) {
			facing = movingDirection =  UP;
			if (gridY > 0 && !(grid->walls[gridX][gridY-1] != -1 && grid->walls[gridX][gridY-1] != playerNum)) {
				startedMoving = hge->Timer_GetTime();
			}
		}

		//Set animation in the right direction!
		if (oldDir == DOWN) {
			if (facing == RIGHT) {
				botonoidGraphics[whichBotonoid]->SetMode(HGEANIM_REV | HGEANIM_LOOP);
			} else if (facing == LEFT || facing == UP) {
				botonoidGraphics[whichBotonoid]->SetMode(HGEANIM_FWD | HGEANIM_LOOP);
			}
		}
		if (oldDir == RIGHT) {
			if (facing == DOWN || facing == LEFT) {
				botonoidGraphics[whichBotonoid]->SetMode(HGEANIM_FWD | HGEANIM_LOOP);
			} else if (facing == UP) {
				botonoidGraphics[whichBotonoid]->SetMode(HGEANIM_REV | HGEANIM_LOOP);
			}
		}
		if (oldDir == LEFT) {
			if (facing == DOWN) {
				botonoidGraphics[whichBotonoid]->SetMode(HGEANIM_REV | HGEANIM_LOOP);
			} else if (facing == UP || facing == RIGHT) {
				botonoidGraphics[whichBotonoid]->SetMode(HGEANIM_FWD | HGEANIM_LOOP);
			}
		}
		if (oldDir == UP) {
			if (facing == LEFT) {
				botonoidGraphics[whichBotonoid]->SetMode(HGEANIM_REV | HGEANIM_LOOP);
			} else if (facing == RIGHT || facing == DOWN) {
				botonoidGraphics[whichBotonoid]->SetMode(HGEANIM_FWD | HGEANIM_LOOP);
			}
		}
		if (oldDir != facing) botonoidGraphics[whichBotonoid]->SetFrame(oldDir*4);

	}

	
	//Action key
	if (hge->Input_GetKeyState(playerControls[playerNum].changeColor)) {

		//Start color change mode
		if (!colorChangeMode && !foundationMode) {
			if (!foundationMode && !colorChangeMode && hge->Timer_GetTime() > endedColorChange + COLOR_CHANGE_REFRESH) {
				colorChangeMode = true;
				numChangesLeft = 5;
			}

		//Build wall
		} else if (foundationMode) {

			//If the player is on a foundation, build a wall.
			if (grid->buildWall(gridX,gridY,playerNum)) {
				numWallsLeft--;
			}

			//No walls left to build
			if (numWallsLeft == 0) {
				foundationMode = false;
				grid->clearFoundations(playerNum);
			}	 
		}
	}

	//temp
	if (hge->Input_KeyDown(HGEK_G)) {
		grid->foundations[gridX][gridY] = playerNum;
		grid->buildWall(gridX, gridY, playerNum);
	}

}

void Player::draw(float dt) {

	//Botonoid sprite
	if (botonoidGraphics[whichBotonoid]->GetFrame() != facing*4) {
		botonoidGraphics[whichBotonoid]->Update(dt);
	}
	botonoidGraphics[whichBotonoid]->Render(x,y);

	//Number of color changes left
	if (colorChangeMode) {
		resources->GetFont("timer")->printf(x,y-40,HGETEXT_CENTER, "%d", numChangesLeft);
	}

	//Number of walls left
	if (foundationMode) {
		resources->GetFont("timer")->SetColor(ARGB(255,0,255,0));
		resources->GetFont("timer")->printf(x,y-20,HGETEXT_CENTER, "%d", numWallsLeft);
		resources->GetFont("timer")->SetColor(ARGB(255,255,255,255));
	}

	//Color change bar
	if (hge->Timer_GetTime() < endedColorChange + COLOR_CHANGE_REFRESH) {
		float length = ((COLOR_CHANGE_REFRESH-(hge->Timer_GetTime() - endedColorChange))/COLOR_CHANGE_REFRESH) * 32.0f;
		gui->barSprites[playerNum]->RenderStretch(x-16.0f,y-25.0f,x-16.0f+length,y-15.0f);
	}

}

/**
 * Update color change stuff
 */ 
void Player::doColorChanging() {

	//Change current square
	if (!foundationMode && colorChangeMode) {

		//Start changing the color at the current square if its not already changing
		grid->startColorChangeAt(gridX, gridY, playerNum);
		numChangesLeft--;

	}

	//End color change mode
	if (numChangesLeft == 0) {
		colorChangeMode = false;
		endedColorChange = hge->Timer_GetTime();
	}
}

/**
 * Enter foundation mode.
 */
void Player::startFoundationMode(int _numWalls) {

	//Stop color change mode and start foundation mode
	foundationMode = true;
	numWallsLeft = _numWalls;

}