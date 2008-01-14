#include "game.h"

//Global stuff
extern HGE *hge;
extern Grid *grid;
extern GUI *gui;
extern hgeAnimation *botonoidGraphics[NUM_BOTONOIDS];
extern hgeResourceManager *resources;
extern hgeFont *debugFont;
extern Input *input;
extern StatsPage *statsPage;
extern GameInfo gameInfo;
extern ItemManager *itemManager;
extern Player *players[3];

/**
 * Constructor
 */
Player::Player(int _x, int _y, int _playerNum, int _whichBotonoid) {

	//Set initial state
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
	colorChangeMode = foundationMode = buildWallPressed = false;
	startedMoving = -10.0f;
	endedColorChange = -10.0f;
	timeChangedItem = -10.0f;
	timeToMove = (GRID_SIZE+1) / speed;
	collisionBox = new hgeRect();
	collisionBox->SetRadius(x,y,14.0f);

	//Center of the wheel
	itemWheelX = 928.0f;
	itemWheelY = 320.0f + playerNum * 185.0f;

	//Reset the player's animation to face down
	botonoidGraphics[whichBotonoid]->SetFrame(0);
	
	//Item slots
	for (int i = 0; i < 4; i++) {
		positionAngles[i] = (1.5f*PI) + (0.5f * PI *(float)i);
		itemSlots[i].position = i;
		itemSlots[i].angle = itemSlots[i].targetAngle = positionAngles[itemSlots[i].position];
		itemSlots[i].code = EMPTY;
		itemSlots[i].quantity = 0;
	}

	//Empty item slot
	if (whichBotonoid == 0) {
		emptyItemSlot = resources->GetSprite("emptySlotGold");
	} else if (whichBotonoid == 1) {
		emptyItemSlot = resources->GetSprite("emptySlotBlack");
	} else if (whichBotonoid == 2) {
		emptyItemSlot = resources->GetSprite("emptySlotWhite");
	}

	//temp
	itemSlots[0].code = ITEM_SILLY_PAD;
	itemSlots[0].quantity = 10;

}

/**
 * Destructor
 */
Player::~Player() {
	delete collisionBox;
	delete emptyItemSlot;
}

/**
 * This is the main update function for the Player that is called every frame by HGE.
 */
void Player::update(float dt) {

	//Find current grid location
	gridX = (x - grid->xOffset) / (GRID_SIZE+1);
	gridY = (y - grid->yOffset) / (GRID_SIZE+1);

	//Update collision box
	collisionBox->SetRadius(x, y, 14.0f);

	doColorChanging();
	doMovement(dt);
	doStats(dt);
	updateItemSlots(dt);

	//Update previous position - this must be done after doColorChanging() is called.
	lastGridX = gridX;
	lastGridY = gridY;
	
	//Action key - keep track of whether or not the action button has been pressed to
	// build a wall so that the player build walls by moving and holding down the action
	// button, but when the last wall is placed it won't automatically enter color change
	// mode. (Fix for Issue 5)
	if (!input->buttonDown(INPUT_ACTION, playerNum)) buildWallPressed = false;
	if (input->buttonDown(INPUT_ACTION, playerNum)) {

		//Start color change mode
		if (!colorChangeMode && !foundationMode && !buildWallPressed) {
			if (!foundationMode && !colorChangeMode && hge->Timer_GetTime() > endedColorChange + COLOR_CHANGE_REFRESH) {
				colorChangeMode = true;
				numChangesLeft = 5;
			}

		//Build wall
		} else if (foundationMode) {

			//Remember that action button was pressed to build a wall (see comment above)
			buildWallPressed = true;

			//If the player is on a foundation, build a wall.
			if (grid->buildWall(gridX,gridY,playerNum)) {
				numWallsLeft--;
			}

			//No walls left to build
			if (numWallsLeft == 0 || grid->numFoundations(playerNum) == 0) {
				numWallsLeft = 0;
				foundationMode = false;
				grid->clearFoundations(playerNum);
			}	 
		}
	}

	//Use Item key
	if (input->buttonPressed(INPUT_ITEM, playerNum)) {
		useItem(dt);
	}

	//temp debug input
	if (hge->Input_KeyDown(HGEK_G)) {
		grid->foundations[gridX][gridY] = playerNum;
		grid->buildWall(gridX, gridY, playerNum);
	}
	if (hge->Input_KeyDown(HGEK_F)) {
		grid->foundations[gridX][gridY] = playerNum;
	}

} //end update()

void Player::draw(float dt) {

	//Draw the botonoid animation. Only update the animation if it is not set to the frame
	// corresponding to the botonoid's correct direction. This makes the botonoid turn to
	// its current direction then display a still image.
	if (botonoidGraphics[whichBotonoid]->GetFrame() != facing*4) {
		botonoidGraphics[whichBotonoid]->Update(dt);
	}
	botonoidGraphics[whichBotonoid]->Render(x,y);

	//Draw the number of color changes remaining to the left of the Botonoid
	if (colorChangeMode) {
		resources->GetFont("timer")->printf(x - 16.0f, y-13.0f, HGETEXT_RIGHT, "%d", numChangesLeft);
	}

	//Drew the number of walls remaining to the right of the Botonoid
	if (foundationMode) {
		resources->GetFont("timer")->printf(x + 16.0f, y-13.0f, HGETEXT_LEFT, "%d", numWallsLeft);
	}

	//Color change bar
	if (hge->Timer_GetTime() < endedColorChange + COLOR_CHANGE_REFRESH) {

		//Determine where to draw the bar based on botonoid model
		float barY = y-28.0f;
		if (whichBotonoid == 1) barY = y - 35.0f;		//Barvinoid

		//Draw the bar
		float length = ((COLOR_CHANGE_REFRESH-(hge->Timer_GetTime() - endedColorChange))/COLOR_CHANGE_REFRESH) * 32.0f;
		gui->barSprites[whichBotonoid]->RenderStretch(x-16.0f,				//x0
													  barY,					//y0
													  x-16.0f+length,		//x1
													  barY+10.0f);			//y1

	}

	drawItemWheel(dt);

} //end draw()

/**
 * Update color changing stuff. This is called every frame from the update() method.
 */ 
void Player::doColorChanging() {

	//Start a new color change every time the player enters a new square while in
	// color change mode.
	if ((gridX != lastGridX || gridY != lastGridY) && colorChangeMode) {

		//Change current square
		if (!foundationMode && colorChangeMode) {

			//Decrease color change counter if the player moves onto any colored square, even if its
			// already changing (meaning not if wall/foundation/gardens)
			if (grid->walls[gridX][gridY] == -1 && grid->foundations[gridX][gridY] == -1 && grid->gardens[gridX][gridY] == -1) {
				numChangesLeft--;
			}

			//Start changing the color at the current square if its not already changing
			grid->startColorChangeAt(gridX, gridY, playerNum);

		}

		//End color change mode
		if (numChangesLeft == 0) {
			colorChangeMode = false;
			endedColorChange = hge->Timer_GetTime();
		}

	}
} //end doColorChanging()

/**
 * Enter foundation mode.
 */
void Player::startFoundationMode(int _numWalls) {

	//Stop color change mode and start foundation mode
	foundationMode = true;
	numWallsLeft = _numWalls;

}

/**
 * Updates stats for this player that will show when the game ends. This is called
 * every frame from the update() method.
 */
void Player::doStats(float dt) {

	//Count score and walls/gardens
	score = 0;
	statsPage->stats[playerNum].wallsBuilt = 0;
	statsPage->stats[playerNum].gardensBuilt = 0;
	for (int i = 0; i < grid->width; i++) {
		for (int j = 0; j < grid->height; j++) {
			if (grid->walls[i][j] == playerNum) {
				score += 1;
				statsPage->stats[playerNum].wallsBuilt++;
			}
			if (grid->gardens[i][j] == playerNum) {
				score += 2;
				statsPage->stats[playerNum].gardensBuilt++;
			}
		}
	}

	//Determine winner
	int maxScore = -1;
	for (int i = 0; i < gameInfo.numPlayers; i++) {
		if (players[i]->score > maxScore) {
			//Player i is the winner
			gameInfo.winner = i;
			maxScore = players[i]->score;
		} else if (players[i]->score == maxScore) {
			//Tie
			gameInfo.winner = -1;
		}
	}

	//Time winning
	if (gameInfo.winner == playerNum) {
		statsPage->stats[playerNum].timeWinning += dt;
	}

} //end doStats()

/**
 * Update movement related stuff - apply velocity for this frame, detect and process 
 * new movement, and update turning animations. This is called every frame from the
 * update() method.
 */
void Player::doMovement(float dt) {

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
		if (input->buttonDown(INPUT_LEFT, playerNum)) {
			facing = movingDirection = LEFT;
			if (gridX > 0 && !(grid->walls[gridX-1][gridY] != -1 && grid->walls[gridX-1][gridY] != playerNum)
					&& !(grid->sillyPads[gridX-1][gridY] != -1 && grid->sillyPads[gridX-1][gridY] != playerNum)) {
				startedMoving = hge->Timer_GetTime();
			}
		//Right
		} else if (input->buttonDown(INPUT_RIGHT, playerNum)) {
			facing = movingDirection =  RIGHT;
			if (gridX < grid->width-1 && !(grid->walls[gridX+1][gridY] != -1 && grid->walls[gridX+1][gridY] != playerNum)
					&& !(grid->sillyPads[gridX+1][gridY] != -1 && grid->sillyPads[gridX+1][gridY] != playerNum)) {
				startedMoving = hge->Timer_GetTime();
			}
		//Down
		} else if (input->buttonDown(INPUT_DOWN, playerNum)) {
			facing = movingDirection =  DOWN;
			if (gridY < grid->height-1 && !(grid->walls[gridX][gridY+1] != -1 && grid->walls[gridX][gridY+1] != playerNum)
					&& !(grid->sillyPads[gridX][gridY+1] != -1 && grid->sillyPads[gridX][gridY+1] != playerNum)) {
				startedMoving = hge->Timer_GetTime();
			}
		//Up
		} else if (input->buttonDown(INPUT_UP, playerNum)) {
			facing = movingDirection =  UP;
			if (gridY > 0 && !(grid->walls[gridX][gridY-1] != -1 && grid->walls[gridX][gridY-1] != playerNum)
					&& !(grid->sillyPads[gridX][gridY-1] != -1 && grid->sillyPads[gridX][gridY-1] != playerNum)) {
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
} //end doMovement()

/**
 * Add an item to the player's item wheel. Returns whether or not there was room.
 */
bool Player::addItem(int item) {

	//Loop through the item wheel. If a slot already has the item, add to its quantity
	for (int i = 0; i < 4; i++) {
		if (itemSlots[i].code == item) {
			itemSlots[i].quantity++;
			return true;
		}
	}

	//If the player doesn't already have the item, add it to the first empty slot
	for (int i = 0; i < 4; i++) {
		if (itemSlots[i].code == EMPTY) {
			itemSlots[i].code = item;
			itemSlots[i].quantity = 1;
			return true;
		}
	}

	//Return false if there was no room for the item
	return false;

}

/**
 * Updates the Player's item slots in the GUI. Called every frame from the update()
 * method.
 */
void Player::updateItemSlots(float dt) {

	int numChanges = 1;
	int currentSlot = 0;
	bool keepTurning = true;

	//Change to next item
	if (input->buttonPressed(INPUT_NEXT_ITEM, playerNum) && hge->Timer_GetTime() > timeChangedItem + SPIN_TIME) {

		//Determine how many turns to get to the next item
		currentSlot = LEFT_SLOT;
		numChanges = 1;
		keepTurning = true;
		while (currentSlot > TOP_SLOT && keepTurning) {
			if (itemInSlot(currentSlot) == EMPTY) {
				numChanges++;
				currentSlot--;
			} else {
				keepTurning = false;
			}
		}
		
		//Turn that number of times
		for (int j = 0; j < numChanges; j++) {
			for (int i = 0; i < 4; i++) {
				if (itemSlots[i].position == 3) itemSlots[i].position = 0;
				else itemSlots[i].position++;
				itemSlots[i].targetAngle += (PI/2.0f);
			}
		}
		timeChangedItem = hge->Timer_GetTime() + SPIN_TIME*(float)numChanges;
	}

	//Change to previous item
	if (input->buttonPressed(INPUT_LAST_ITEM, playerNum) && hge->Timer_GetTime() > timeChangedItem + SPIN_TIME) {

		//Determine how many turns to get to the next item
		currentSlot = RIGHT_SLOT;
		numChanges = 1;
		keepTurning = true;
		while (currentSlot <= LEFT_SLOT && keepTurning) {
			if (itemInSlot(currentSlot) == EMPTY) {
				numChanges++;
				currentSlot++;
			} else {
				keepTurning = false;
			}
		}
		
		//Turn that number of times
		for (int j = 0; j < numChanges; j++) {
			for (int i = 0; i < 4; i++) {
				if (itemSlots[i].position == 0) itemSlots[i].position = 3;
				else itemSlots[i].position--;
				itemSlots[i].targetAngle -= (PI/2.0f);
			}
		}
		timeChangedItem = hge->Timer_GetTime();
	}

	//Update angles
	float speed = (PI/2.0f)/SPIN_TIME;
	float angle, targetAngle;
	for (int i = 0; i < 4; i++) {

		angle = itemSlots[i].angle;
		targetAngle = itemSlots[i].targetAngle;

		//Set to target angle once its close
		if (angle > targetAngle - 0.05f && angle < targetAngle + 0.05f) {
			itemSlots[i].angle = itemSlots[i].targetAngle;

		//Spin right
		} else if (angle < targetAngle) {
			itemSlots[i].angle += speed*dt;

		//Spin left
		} else if (angle > targetAngle) {
			itemSlots[i].angle -= speed*dt;
		}

		itemSlots[i].x = itemWheelX+ 35.0f * cos(itemSlots[i].angle);
		itemSlots[i].y = itemWheelY + 25.0f * sin(itemSlots[i].angle);

	}

}

/**
 * Draws the player's item wheel in the GUI
 */
void Player::drawItemWheel(float dt) {

	//Draw the selected item cursor - it is always the top item
	resources->GetSprite("itemcursor")->Render(itemWheelX + 8.0, itemWheelY - 40.0);

	//Loop through the slots
	for (int i = 0; i < 4; i++) {

		//If the slot isn't empty, draw the item in it.
		if (itemSlots[i].code != EMPTY) {
			itemManager->itemSprites[itemSlots[i].code]->Render(itemSlots[i].x, itemSlots[i].y);
			
			//If there is more than 1 of this item, draw the quantity
			if (itemSlots[i].quantity > 1) {
				resources->GetFont("timer")->SetScale(0.6f);
				resources->GetFont("timer")->printf(itemSlots[i].x + 28.0f, itemSlots[i].y-4.0f, HGETEXT_LEFT, "%d", itemSlots[i].quantity);
				resources->GetFont("timer")->SetScale(1.0f);
			}

		} else {
			//If the slot is empty, draw the empty item sprite
			emptyItemSlot->Render(itemSlots[i].x, itemSlots[i].y);
		}

	}
} //end drawItemWheel()

/**
 * Uses the currently selected item
 */
void Player::useItem(float dt) {

	//Determine which item is in the top slot
	int item;
	for (int i = 0; i < 4; i++) {
		if (itemSlots[i].position == TOP_SLOT) item = itemSlots[i].code;
	}

	//Silly Pad
	if (item == ITEM_SILLY_PAD) {
		grid->placeSillyPad(gridX, gridY, playerNum);
	
	//Super Wall
	} else if (item == ITEM_SUPER_WALL) {
		grid->placeSuperWall(gridX, gridY, playerNum);

	//Super Flower
	} else if (item == ITEM_SUPER_FLOWER) {
		grid->placeSuperFlower(gridX, gridY, playerNum);
	}

	//Decrease quantity of whatever item was just used
	if (item != EMPTY) {
		if (itemSlots[TOP_SLOT].quantity == 1) {
			itemSlots[TOP_SLOT].quantity = 0;
			itemSlots[TOP_SLOT].code = EMPTY;
		} else {
			itemSlots[TOP_SLOT].quantity--;
		}
	}

} //end useItem()

/**
 * Returns what item is in the specified slot.
 */
int Player::itemInSlot(int slot) {
	for (int i = 0; i < 4; i++) {
		if (itemSlots[i].position == slot) return itemSlots[i].code;
	}
	return EMPTY;
}

