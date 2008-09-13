#include "game.h"
#include "WallBreakerManager.h"
#include "MissileManager.h"

//Global stuff
extern HGE *hge;
extern Grid *grid;
extern GUI *gui;
extern hgeAnimation *botonoidGraphics[NUM_BOTONOIDS];
extern hgeResourceManager *resources;
extern hgeAnimation *itemAnimations[NUM_ITEMS-1];
extern Input *input;
extern StatsPage *statsPage;
extern GameInfo gameInfo;
extern WallBreakerManager *wallBreakerManager;
extern MissileManager *missileManager;
extern ItemManager *itemManager;
extern Player *players[3];
extern float gameTime;

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
	score = 0;
	health = 3;
	colorChangeMode = foundationMode = buildWallPressed = false;
	startedMoving = -10.0;
	endedColorChange = -10.0;
	timePunched = -10.0;
	timeGhosted = -10.0;
	collisionBox = new hgeRect();
	collisionBox->SetRadius(x,y,14.0f);
	punchingGlove = resources->GetAnimation("punchingglove");
	slowed = false;
	punching = false;
	ghostMode = false;
	dead = false;

	//Center of the wheel
	itemWheelX = 926.0;
	itemWheelY = 320.0 + playerNum * 183.0;

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

	//Item slot 1 is always the GHOST
	itemSlots[0].code = ITEM_GHOST;
	itemSlots[0].quantity = 0;

	//temp
	itemSlots[1].code = ITEM_PUNCH_GLOVE;
	itemSlots[1].quantity = 200;
	itemSlots[2].code = ITEM_MISSILE;
	itemSlots[2].quantity = 50;
}

/**
 * Destructor
 */
Player::~Player() {
	delete collisionBox;
	delete emptyItemSlot;
	delete slowEffectParticle;
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
	updatePunchingGlove(dt);

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

	//Use Item
	if (input->buttonPressed(INPUT_ITEM, playerNum)) useItem(dt);

	//Update state
	if (slowed && gameTime > timeSlowed + SLOW_DURATION) slowed = false;
	if (ghostMode && gameTime > timeGhosted + GHOST_DURATION) {
		ghostMode = false;
		if (dead) {
			dead = false;
			health = 3.0;
		}
	}


	//----temp debug input------------
	if (hge->Input_KeyDown(HGEK_G)) {
		grid->foundations[gridX][gridY] = playerNum;
		grid->buildWall(gridX, gridY, playerNum);
	}
	if (hge->Input_KeyDown(HGEK_F)) {
		grid->foundations[gridX][gridY] = playerNum;
	}
	//--------------------------------

} //end update()

void Player::draw(float dt) {

	//Draw the slowed particle effect
	if (slowed) {
		slowEffectParticle->MoveTo(x,y);
		slowEffectParticle->Update(dt);
		slowEffectParticle->Render();
	}

	//Draw Punch glove UP before the botonoid
	/**
	if (punching) {
		if (facing == UP) {
			punchingGlove->RenderEx(x-10.0, y+5.0, punchingGloveAngle);
		}
	}*/

	//Draw the botonoid animation. Only update the animation if it is not set to the frame
	// corresponding to the botonoid's correct direction. This makes the botonoid turn to
	// its current direction then display a still image.
	if (botonoidGraphics[whichBotonoid]->GetFrame() != facing*4) {
		botonoidGraphics[whichBotonoid]->Update(dt);
	}
	if (ghostMode) botonoidGraphics[whichBotonoid]->SetColor(ARGB(125.0,255.0,255.0,255.0));
	botonoidGraphics[whichBotonoid]->Render(x,y);
	if (ghostMode) botonoidGraphics[whichBotonoid]->SetColor(ARGB(255.0,255.0,255.0,255.0));

	//Draw Punch glove LEFT, RIGHT, DOWN after the botonoid
	if (punching) {
		punchingGlove->RenderEx(x,y, punchingGloveAngle);
		/**
		if (facing == LEFT ||) {
			punchingGlove->RenderEx(x, y+10.0, PI);
		} else if (facing == RIGHT) {
			punchingGlove->Render(x,y-10.0);
		} else if (facing == DOWN) {
			punchingGlove->RenderEx(x+10.0, y+5.0, PI/2.0);
		}*/
	}

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
				score += gameInfo.pointsPerWall[playerNum];
				statsPage->stats[playerNum].wallsBuilt++;
			}
			if (grid->gardens[i][j] == playerNum) {
				score += gameInfo.pointsPerGarden[playerNum];
				statsPage->stats[playerNum].gardensBuilt++;
			}
			if (grid->superFlowers[i][j] == playerNum) {
				score += 15;
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
	float speed = (slowed ? SLOWED_SPEED : SPEED);
	float timeToMove = (GRID_SIZE+1) / speed;
	
	if (hge->Timer_GetTime() < startedMoving + timeToMove) {
		switch (movingDirection) {
			case UP: y -= speed*dt; break;
			case DOWN: y += speed*dt; break;
			case LEFT: x -= speed*dt; break;
			case RIGHT: x += speed*dt; break;
		}

	//Start movement
	} else {

		//Make sure the player is centered in their current grid square because 
		//they are done their last move.
		x = grid->xOffset + gridX*(GRID_SIZE+1) + (GRID_SIZE+1)/2;
		y = grid->yOffset + gridY*(GRID_SIZE+1) + (GRID_SIZE+1)/2;

		int oldDir = facing;

		//Left
		if (input->buttonDown(INPUT_LEFT, playerNum)) {
			facing = movingDirection = LEFT;
			if (gridX > 0 && !collisionAt(gridX-1, gridY)) {
				startedMoving = hge->Timer_GetTime();
			}
		//Right
		} else if (input->buttonDown(INPUT_RIGHT, playerNum)) {
			facing = movingDirection =  RIGHT;
			if (gridX < grid->width-1 && !collisionAt(gridX + 1, gridY)) { 
				startedMoving = hge->Timer_GetTime();
			}
		//Down
		} else if (input->buttonDown(INPUT_DOWN, playerNum)) {
			facing = movingDirection =  DOWN;
			if (gridY < grid->height-1 && !collisionAt(gridX, gridY+1)) {
				startedMoving = hge->Timer_GetTime();
			}
		//Up
		} else if (input->buttonDown(INPUT_UP, playerNum)) {
			facing = movingDirection =  UP;
			if (gridY > 0 && !collisionAt(gridX, gridY-1)) {
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
			itemSlots[i].quantity += getItemQuantityPerToken(item);
			return true;
		}
	}

	//If the player doesn't already have the item, add it to the first empty slot
	for (int i = 0; i < 4; i++) {
		if (itemSlots[i].code == EMPTY) {
			itemSlots[i].code = item;
			itemSlots[i].quantity = getItemQuantityPerToken(item);
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

	//Determine if the player is allowed to spin the wheel right now. Don't allow them
	//to if it is already spinning, or if there is only one item and its already
	//selected.
	bool allowSpin = true;
	if (itemSlots[0].angle != itemSlots[0].targetAngle) allowSpin = false;
	if (numEmptyItemSlots() == 3 && itemInSlot(TOP_SLOT) != EMPTY) allowSpin = false;
	if (numEmptyItemSlots() == 4) allowSpin = false;

	//Handle change item input
	if (allowSpin && input->buttonDown(INPUT_NEXT_ITEM, playerNum)) {

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
	}

	//Change to previous item
	if (allowSpin && input->buttonDown(INPUT_LAST_ITEM, playerNum)) {

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
	}

	//Update angles
	float moveDist = ((PI/2.0)/SPIN_TIME) * dt;

	//We only need to look at one item slot because they all move together.
	//Spin right
	if (itemSlots[0].angle < itemSlots[0].targetAngle) {
		for (int i = 0; i < 4; i++) itemSlots[i].angle += moveDist;
		//Once one item slot gets to the target angle, the rest of them
		//should be there too
		if (itemSlots[0].angle >= itemSlots[0].targetAngle) {
			for (int i = 0; i < 4; i++) itemSlots[i].angle = itemSlots[i].targetAngle;
		}
	}

	//Spin left
	if (itemSlots[0].angle > itemSlots[0].targetAngle) {
		for (int i = 0; i < 4; i++) itemSlots[i].angle -= moveDist;
		//Once one item slot gets to the target angle, the rest of them
		//should be there too
		if (itemSlots[0].angle <= itemSlots[0].targetAngle) {
			for (int i = 0; i < 4; i++) itemSlots[i].angle = itemSlots[i].targetAngle;
		}
	}

	//Update the new x,y coordinates of each itemslot
	for (int i = 0; i < 4; i++) {

		itemSlots[i].x = itemWheelX + 35.0f * cos(itemSlots[i].angle);
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

		if (itemSlots[i].code == ITEM_GHOST) {

			resources->GetSprite("ghost")->Render(itemSlots[i].x, itemSlots[i].y);

			if (ghostMode) {
				resources->GetFont("timer")->SetScale(0.8);
				resources->GetFont("timer")->SetColor(ARGB(255,0,0,0));
				resources->GetFont("timer")->printf(itemSlots[i].x+18.0, itemSlots[i].y + 10.0, HGETEXT_CENTER, "%d", (int)((timeGhosted + GHOST_DURATION) - gameTime));
				resources->GetFont("timer")->SetScale(1.0);
				resources->GetFont("timer")->SetColor(ARGB(255,255,255,255));
			}

		} else if (itemSlots[i].code != EMPTY) {

			itemAnimations[itemSlots[i].code]->SetFrame(0);
			itemAnimations[itemSlots[i].code]->Render(itemSlots[i].x+16, itemSlots[i].y+16);
			
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

	//You can't use items in ghost mode!
	if (ghostMode) return;

	//Whether or not the item was successfully used.
	bool itemUsed = false;

	int topSlot;
	int item;
	for (int i = 0; i < 4; i++) {
		if (itemSlots[i].position == TOP_SLOT) {
			topSlot = i;		
			item = itemSlots[i].code;
		}
	}

	//Silly Pad
	if (item == ITEM_SILLY_PAD) {
		itemUsed = grid->placeSillyPad(gridX, gridY, playerNum);
	
	//Ghost Mode
	} else if (item == ITEM_GHOST) {
		//Don't set itemUsed to true because the player has unlimited ghosts
		if (!ghostMode) {
			ghostMode = true;
			timeGhosted = gameTime;
		}

	//Heart
	} else if (item == ITEM_HEALTH) {
		if (health < 3.0) {
			health += 1.0;
			itemUsed = true;
		}

	//Enemy slow
	} else if (item == ITEM_SLOW) {
		itemUsed = true;
		for (int i = 0; i < gameInfo.numPlayers; i++) {
			if (playerNum != i) players[i]->slowPlayer();
		}

	//Super Wall
	} else if (item == ITEM_SUPER_WALL) {
		itemUsed = grid->placeSuperWall(gridX, gridY, playerNum);

	//Super Flower
	} else if (item == ITEM_SUPER_FLOWER) {
		itemUsed = grid->placeSuperFlower(gridX, gridY, playerNum);

	//Punching Glove
	} else if (item == ITEM_PUNCH_GLOVE) {
		punchingGlove->SetFrame(0);
		punchingGlove->Play();
		timePunched = gameTime;
		punchingGloveTarget = findClosestEnemy();
		itemUsed = true;

	//Missile
	} else if (item == ITEM_MISSILE) {
		itemUsed = true;
		missileManager->addMissile(playerNum, gridX, gridY);
		hge->Effect_PlayEx(resources->GetEffect("snd_missile"), 50);

	//Wall Breaker
	} else if (item == ITEM_WALLBREAKER) {
		if (facing == LEFT && grid->isOtherPlayersWallAt(gridX-1, gridY, playerNum)) {
			wallBreakerManager->addWallBreaker(playerNum, gridX-1, gridY);
			itemUsed = true;
		} else if (facing == RIGHT && grid->isOtherPlayersWallAt(gridX+1, gridY, playerNum)) {
			wallBreakerManager->addWallBreaker(playerNum, gridX+1, gridY);
			itemUsed = true;
		} else if (facing == UP && grid->isOtherPlayersWallAt(gridX, gridY-1, playerNum)) {
			wallBreakerManager->addWallBreaker(playerNum, gridX, gridY-1);
			itemUsed = true;
		} else if (facing == DOWN && grid->isOtherPlayersWallAt(gridX, gridY+1, playerNum)) {
			wallBreakerManager->addWallBreaker(playerNum, gridX, gridY+1);
			itemUsed = true;
		}
	}

	if (itemUsed) {

		//Count item used
		statsPage->stats[playerNum].numItemsUsed++;

		//Decrease quantity of whatever item was just used
		if (item != EMPTY) {
			if (itemSlots[topSlot].quantity == 1) {
				itemSlots[topSlot].quantity = 0;
				itemSlots[topSlot].code = EMPTY;
			} else {
				itemSlots[topSlot].quantity--;
			}
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

/**
 * Returns the number of each item the player gets when they pick up
 * the token representing the item.
 */
int Player::getItemQuantityPerToken(int item) {
	switch (item) {
		case ITEM_SILLY_PAD:
			return 5;
		case ITEM_SUPER_WALL:
			return 5;
		default:
			return 1;
	}
}

/**
 * Returns the number of item slots that are currently empty.
 */
int Player::numEmptyItemSlots() {
	int num = 0;
	for (int i = 0; i < 4; i++) {
		if (itemInSlot(i) == EMPTY) num++;
	}
	return num;
}

/**
 * Deals the specified amount of damage to the player.
 */
void Player::dealDamage(float damage) {

	health -= damage;

	if (health <= 0.0) {
		health = 0.0;
		die();
	}

}

/**
 * Uses the slow player item on this player
 */
void Player::slowPlayer() {

	//Create the slow effect particle
	slowEffectParticle = new hgeParticleSystem(&resources->GetParticleSystem("slow")->info);
	slowEffectParticle->FireAt(x, y);

	slowed = true;
	timeSlowed = gameTime;
	
}

/**
 * Returns whether or not the player would collide with something at square (x,y)
 */
bool Player::collisionAt(int gX, int gY) {
	if (ghostMode) return false;
	return ((grid->walls[gX][gY] != -1 && 
			grid->walls[gX][gY] != playerNum) || 
			(grid->sillyPads[gX][gY] != -1 && 
			grid->sillyPads[gX][gY] != playerNum));
}

/**
 * Called when the player's health reached 0.
 */
void Player::die() {

	//The player turns into a ghost when they die
	ghostMode = true;
	dead = true;
	timeGhosted = gameTime + 15.0;

	//hge->Effect_Play(resources->GetEffect("snd_explosion1"));
}

/**
 * Calculates shit related to the punching glove
 */
void Player::updatePunchingGlove(float dt) {

	//First determine if the player is currently punching
	punching = (gameTime > timePunched  && gameTime < timePunched + PUNCH_DELAY);
	if (!punching) return;

	punchingGlove->Update(dt);

	//Find the angle between the player and his target
	punchingGloveAngle = atan((y - players[punchingGloveTarget]->y) /
							  (x - players[punchingGloveTarget]->x));

	if (x < players[punchingGloveTarget]->x) {
		//Don't need to change anything!
	} else if (x > players[punchingGloveTarget]->x) {
		//To the right of the target, angle will be off by PI
		punchingGloveAngle += PI;
	} else if (players[punchingGloveTarget]->y < y) {
		//Directly above target, angle will be off by PI
		punchingGloveAngle += PI;
	} else if (players[punchingGloveTarget]->y > y) {
		//Directly below target, angle will be off by PI
		punchingGloveAngle += PI;
	}
}

/**
 * Returns the number of the closest enemy player
 */
int Player::findClosestEnemy() {

	float dist;
	float minDistance = 999999.0;
	int closestPlayer = -1;

	for (int i = 0; i < gameInfo.numPlayers; i++) {
		if (i != playerNum) {
			dist = distance(x, y, players[i]->x, players[i]->y);
			if (dist < minDistance) {
				minDistance = dist;
				closestPlayer = i;
			}
		}
	}

	return closestPlayer;

}

