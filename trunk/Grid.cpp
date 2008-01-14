#include "game.h"

extern HGE *hge;
extern hgeSprite *tileSprites[NUM_COLORS];
extern hgeSprite *botonoidSprites[NUM_BOTONOIDS*NUM_PLAYER_GRAPHICS];
extern hgeSprite *specialTiles[3*4];
extern Player *players[3];
extern hgeResourceManager *resources;
extern hgeFont *debugFont;
extern StatsPage *statsPage;
extern ItemManager *itemManager;
extern float gameTime;

/**
 * Constructor
 */
Grid::Grid(int _width, int _height) {

	height = _height;
	width = _width;
	hge = hgeCreate(HGE_VERSION);

	//Set beginning grid info
	for (int i = 0; i < 32; i++) {
		for (int j = 0; j < 32; j++) {
			startedColorChange[i][j] = 0.0f;
			alpha[i][j] = 0.0f;
			foundations[i][j] = -1;
			walls[i][j] = -1;
			gardens[i][j] = -1;
			sillyPads[i][j] = -1;
			sillyPadsPlaced[i][j] = - 10.0f;
			superWalls[i][j] = -1;
			superFlowers[i][j] = -1;
		}
	}
	resetVisited();
	resetWallsTouched();

	//Generate a random grid
	randomize();

	//Calculate (x,y) of top left of the grid
	xOffset = (1024 - (GRID_SIZE+1)*width) / 2 - (166/2) + 3;
	yOffset = (768 - (GRID_SIZE+1)*height) / 2;

	//Particle managers
	dustClouds = new hgeParticleManager();

	//Load special tiles
	for (int i = 0; i < 3; i++) {
		sillyPadSprites[i] = new hgeSprite(resources->GetTexture("tiles"), 32.0 + 32.0*i, 96.0, 32.0, 32.0);
		superFlowerSprites[i] = new hgeSprite(resources->GetTexture("tiles"), 32.0 + 32.0*i, 128.0, 32.0, 32.0);
		superWallSprites[i] = new hgeSprite(resources->GetTexture("tiles"), 32.0 + 32.0*i, 160.0, 32.0, 32.0);
	}

}

/**
 * Destructor
 */
Grid::~Grid() {
	for (int i = 0; i < 3; i++) {
		delete sillyPadSprites[i];
		delete superWallSprites[i];
		delete superFlowerSprites[i];
	}
}

void Grid::draw(float dt) {
	
	//Draw tiles, foundations and gardens
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {

			float tileX = xOffset + i*(GRID_SIZE+1);
			float tileY = yOffset + j*(GRID_SIZE+1);

			//Foundation
			if (foundations[i][j] >= 0) {
				int index = players[foundations[i][j]]->whichBotonoid*4;
				specialTiles[index]->Render(tileX, tileY);

			//Garden
			} else if (gardens[i][j] >= 0) {
				int index = players[gardens[i][j]]->whichBotonoid*4 + 2;
				specialTiles[index]->Render(tileX, tileY);

			//Colored tile
			} else if (walls[i][j] == -1) {

				//Non-changing tile
				tileSprites[tiles[i][j]]->Render(tileX, tileY);
			
				//If the tile is changing, draw the next tile fading in
				if (startedColorChange[i][j] != 0.0f) {

					int nextTile = (tiles[i][j] == PURPLE) ? RED : tiles[i][j] + 1;
					tileSprites[nextTile]->SetColor(ARGB(alpha[i][j],255,255,255));
					tileSprites[nextTile]->Render(tileX, tileY);
					tileSprites[nextTile]->SetColor(ARGB(255,255,255,255));

					//Draw outline
					hge->Gfx_RenderLine(tileX-1,tileY,tileX+33,tileY);
					hge->Gfx_RenderLine(tileX,tileY,tileX,tileY+33);
					hge->Gfx_RenderLine(tileX+33,tileY,tileX+33,tileY+33);
					hge->Gfx_RenderLine(tileX,tileY+33,tileX+33,tileY+33);

				}

			}

			//Super walls
			if (superWalls[i][j] != -1) {
				superWallSprites[players[superWalls[i][j]]->whichBotonoid]->Render(tileX, tileY);
			}

			//Super flowers
			if (superFlowers[i][j] != -1) {
				superFlowerSprites[players[superFlowers[i][j]]->whichBotonoid]->Render(tileX, tileY);
			}

			//Silly pads
			float timeSincePlaced, alpha;
			if (sillyPads[i][j] != -1) {
				
				//Make the silly pad fade out during the last second
				timeSincePlaced = gameTime - sillyPadsPlaced[i][j];
				if (SILLY_PAD_DURATION - timeSincePlaced < 1.0f) {
					alpha = (1.0f - (1.0f - (SILLY_PAD_DURATION - timeSincePlaced))) / 1.0f * 255.0f;
					sillyPadSprites[sillyPads[i][j]]->SetColor(ARGB(alpha, 255, 255, 255));
				}

				//Draw the silly pad
				sillyPadSprites[sillyPads[i][j]]->Render(tileX, tileY);
				sillyPadSprites[sillyPads[i][j]]->SetColor(ARGB(255,255,255,255));

			}

		}
	}

	//Draw any dust clouds
	dustClouds->Update(dt);
	dustClouds->Render();

	//Draw Walls
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (walls[i][j] >= 0) {
				float tileX = xOffset + i*(GRID_SIZE+1);
				float tileY = yOffset + j*(GRID_SIZE+1);
				specialTiles[players[walls[i][j]]->whichBotonoid*4 + 1]->Render(tileX, tileY);
			}
		}
	}
		
}

/**
 * Update the grid.
 */ 
void Grid::update(float dt) {

	doColorChanges(dt);

	//Loop through each grid square
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			
			//Update silly pads
			if (gameTime > sillyPadsPlaced[i][j] + SILLY_PAD_DURATION) {
				sillyPads[i][j] = -1;
			}

		}
	}

}

/**
 * Fills the grid with random tiles.
 */ 
void Grid::randomize() {

	//Seed the random number generator
	srand(time(NULL));

	//Loop through each tile and assign it a random tile
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			tiles[i][j] = rand() % 5;
		}
	}
}

/**
 * Start the color change process at the specified grid square if its
 * not already changing colors. Return whether or not the process was
 * started.
 */
bool Grid::startColorChangeAt(int gridX, int gridY, int player) {

	bool changed = false;
	
	//Return false if this is anything but a colored tile
	if (foundations[gridX][gridY] != -1) return false;
	if (walls[gridX][gridY] != -1) return false;
	if (tiles[gridX][gridY] > PURPLE) return false;

	//Start changing this tile's color only if its not already being changed.
	if (startedColorChange[gridX][gridY] == 0.0f) {
		
		//Change to next tile
		startedColorChange[gridX][gridY] = hge->Timer_GetTime();

		//Calculate combo size
		resetVisited();
		int comboSize = floodCount(gridX,gridY,nextColor(tiles[gridX][gridY]));

		//If the combo is at least 6 large, make foundations
		if (comboSize >= 6) {
						
			//Update player's max combo size
			if (comboSize > statsPage->stats[player].biggestCombo) 
				statsPage->stats[player].biggestCombo = comboSize;


            //Create the foundations
            for (int i = 0; i < width; i++) {
                for (int j = 0; j < height; j++) {
                    if (visited[i][j]) {
						foundations[i][j] = player;
						gardens[i][j] = -1;
                    }
                }
            }

			//Put the robot in foundation mode
            players[player]->startFoundationMode(comboSize - 3);

		}

		changed = true;
	}

	//Reset the visited array and return whether or not the color
	//change process was started at this square.
	resetVisited();
	return changed;

}

/**
 * Updates color changing related stuff. This method only updates 
 * things related to the graphics. Combos are searched for in 
 * startColorChangeAt()
 */
void Grid::doColorChanges(float dt) {

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {

			//If the current cell is changing colors
			if (startedColorChange[i][j] != 0.0f) {

				alpha[i][j] += (255.0f / COLOR_CHANGE_TIME) * dt;

				//Color changing is done
                if (hge->Timer_GetTime() > startedColorChange[i][j] + COLOR_CHANGE_TIME) {
                            
					//Change to next tile
					tiles[i][j] += 1;
					if (tiles[i][j] > PURPLE) tiles[i][j] = 0;

					//Reset cell info
                    startedColorChange[i][j] = 0.0f;
                    alpha[i][j] = 0.0f;

				}
			}
		}
	}

}

/**
 * Recursively counts the number of adjacent sqaures of the same color
 * starting at (x,y). Returns the number of adjacent sqaures of the same
 * color. Marks all squares that are visited as true in the visited[][] array.
 */
int Grid::floodCount(int x, int y, int color) {
            
		int sum = 1;            //Count this square
		visited[x][y] = true;   //This square has been visited
            
		//Above
		if (y-1 >= 0 && !visited[x][y-1] && getColorAt(x, y-1) == color)
			sum += floodCount(x, y-1, color);
		//Below
		if (y+1 < height && !visited[x][y+1] && getColorAt(x, y+1) == color)
			sum += floodCount(x, y+1, color);
		//Right
		if (x+1 < width && !visited[x+1][y] && getColorAt(x+1, y) == color)
			sum += floodCount(x+1, y, color);
		//Left
		if (x-1 >= 0 && !visited[x-1][y] && getColorAt(x-1, y) == color)
			sum += floodCount(x-1, y, color);
            
		return sum;

}

/**
 * Returns the color at the specified square, or if it is changing, the
 * color it is changing to.
 */
int Grid::getColorAt(int x, int y) {
	if (foundations[x][y] != -1 || walls[x][y] != -1 || gardens[x][y] != -1) return -1;
	if (startedColorChange[x][y] != 0.0f) {
		return (tiles[x][y] == PURPLE) ? RED : tiles[x][y] + 1;
	} else {
		return tiles[x][y];
	}
}


/**
 * Reset the array keeping track of which squares have been visited
 * during recursion.
 */
void Grid::resetVisited() {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			visited[i][j] = false;
		}
	}
}

/**
 * Counts how many outer walls are reachable from square (x,y). It does this
 * by recursively branching out and marking in the wallsTouched[] array which
 * walls were reached. If 2 or less could be reached than an area is enclosed.
 * If the algorithm finds an enemy wall then act as if all 4 walls were touched
 * (and thus a garden can't be made there). This is called for the 4 adjacent
 * squares of a wall when a new wall is built. This recursive algorithm also
 * makes use of the visited[][] array to keep track of which squares have already
 * been searched.
 *
 * @arg	x		X coordinate to start searching from
 * @arg y		Y coordinate to start searching from
 * @arg player	
 */
void Grid::countReachableWalls(int x, int y, int player) {
	
	//Mark this square as visited.
	visited[x][y] = true;

	//Check for enemy walls
	if (walls[x][y] != -1 && walls[x][y] != player) {
		for (int i = 0; i < 4; i++) { wallsTouched[i] = true; }
		return;
	}

	//Check Left
    if (x == 0) {
		wallsTouched[LEFT] = true;
	} else if (!visited[x-1][y] && walls[x-1][y] != player) {
		countReachableWalls(x-1, y, player);
    }

	//Check Right
    if (x == width-1) {
		wallsTouched[RIGHT] = true;
	} else if (!visited[x+1][y] && walls[x+1][y] != player) {
		countReachableWalls(x+1, y, player);
    }

	//Check Up
    if (y == 0) {
		wallsTouched[UP] = true;
	} else if (!visited[x][y-1] && walls[x][y-1] != player) {
		countReachableWalls(x, y-1, player);
    }

	//Check Down
    if (y == height-1) {
		wallsTouched[DOWN] = true;
	} else if (!visited[x][y+1] && walls[x][y+1] != player) {
		countReachableWalls(x, y+1, player);
    }

}

/**
 * Builds a wall by player at (gridX,gridY) if (gridX,gridY) is a foundation.
 * returns whether or not a wall was built.
 */
bool Grid::buildWall(int x, int y, int player) {

	bool wallBuilt = false;

	//If this square has a foundation belonging to player
	if (foundations[x][y] == player && walls[x][y] == -1 && gardens[x][y] == -1) {
		walls[x][y] = player;
		foundations[x][y] = -1;
		wallBuilt = true;

		//Dust cloud!
		dustClouds->SpawnPS(&resources->GetParticleSystem("dust")->info, 
						    xOffset + x*33+10,
							yOffset + y*33+10);
		dustClouds->SpawnPS(&resources->GetParticleSystem("dust")->info, 
						    xOffset + x*33 + 22,
							yOffset + y*33 + 10);
		dustClouds->SpawnPS(&resources->GetParticleSystem("dust")->info, 
						    xOffset + x*33 + 10,
							yOffset + y*33 + 22);
		dustClouds->SpawnPS(&resources->GetParticleSystem("dust")->info, 
						    xOffset + x*33 + 22,
							yOffset + y*33 + 22);

		//If this wall being built enclosed an area then fill it with 
		//gardens!!! Determine if an area is enclosed by calling
		//countReachableWalls() from each adjacent tile. If any of them
		//can reach 2 or less walls than an area must be enclosed, so call
		//the garden flood fill algorithm to fill it with gardens.
		//Only search from an adjacent square if its not out of bounds and
		//theres not a wall on it.

		//Search Right
        if (x < width - 1 && walls[x+1][y] == -1) {
            countReachableWalls(x+1, y, player);
            resetVisited();
			if (numWallsTouched() <= 2) {
				fillGarden(x+1, y, player);
				itemManager->generateItem(x, y, countVisited());
				resetVisited();
			}
            resetWallsTouched();
        }

		//Search Left
        if (x > 0 && walls[x-1][y] == -1) {
            countReachableWalls(x-1, y, player);
            resetVisited();
			if (numWallsTouched() <= 2) {
				fillGarden(x-1, y, player);
				itemManager->generateItem(x, y, countVisited());
				resetVisited();
			}
            resetWallsTouched();
        }

		//Search Up
        if (y > 0 && walls[x][y-1] == -1) {
            countReachableWalls(x, y-1, player);
            resetVisited();
			if (numWallsTouched() <= 2) {
				fillGarden(x, y-1, player);
				itemManager->generateItem(x, y, countVisited());
				resetVisited();
			}
            resetWallsTouched();
        }

		//Search Down
        if (y < height-1 && walls[x][y+1] == -1) {
            countReachableWalls(x, y+1, player);
            resetVisited();
			if (numWallsTouched() <= 2) {
				fillGarden(x, y+1, player);
				itemManager->generateItem(x, y, countVisited());
				resetVisited();
			}
            resetWallsTouched();
        }

	}
		
	return wallBuilt;

}



/**
 * Removes all foundations belonging to player.
 */
void Grid::clearFoundations(int player) {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (foundations[i][j] == player) {
				foundations[i][j] = -1;
			}
		}
	}
}

/**
 * Flood fills an area with a garden for the specified player starting
 * at (x,y).
 */
void Grid::fillGarden(int x, int y, int player) {

	//Stop recursing if this square is a wall
	if (walls[x][y] != -1 || gardens[x][y] != -1) return;

	//Put a garden in this spot
    gardens[x][y] = player;
	foundations[x][y] = -1;
    startedColorChange[x][y] = 0.0f;
    visited[x][y] = true;

    //gardenSound.play(); 
            
	//Recurse right
	if (x < width-1 && !visited[x+1][y]) {
		fillGarden(x+1, y, player);
	}

	//Recurse left
	if (x > 0 && !visited[x-1][y]) {
		fillGarden(x-1, y, player);
	}

	//Recurse up
	if (y > 0 && !visited[x][y-1]) {
		fillGarden(x, y-1, player);
	}

	//Recurse down
	if (y < height-1 && !visited[x][y+1]) {
		fillGarden(x, y+1, player);
	}

}

/**
 * Resets the wallsTouched array to false.
 */
void Grid::resetWallsTouched() {
	for (int i = 0; i < 4; i++) wallsTouched[i] = false;
}

/**
 * Returns the number of trues in wallsTouched[] array
 */
int Grid::numWallsTouched() {
	int numTouched = 0;
	for (int i = 0; i < 4; i++) {
		if (wallsTouched[i]) numTouched++;
	}
	return numTouched;
}

/** 
 * Returns the number of squares marked as visited
 */
int Grid::countVisited() {
	int numVisited = 0;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (visited[i][j]) numVisited++;
		}
	}
	return numVisited;
}

/**
 * Returns whether or not there is a wall at point (x,y)
 */
bool Grid::isWallAt(float x, float y) {
	return (walls[getGridX(x)][getGridY(y)] != -1);
}

/**
 * Returns whether or not (x,y) is on the grid.
 */
bool Grid::inBounds(float x, float y) {
	
	if (x < xOffset) return false;
	if (x > xOffset + 33*width) return false;
	if (y < yOffset) return false;
	if (y > yOffset + 33*height) return false;

	return true;
}

/**
 * Returns the number of foundations on the grid that belong to player.
 */
int Grid::numFoundations(int player) {
	int numFoundations = 0;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (foundations[i][j] == player && gardens[i][j] == -1 && walls[i][j] == -1) numFoundations++;
		}
	}
	return numFoundations;
}

/**
 * Places a silly pad on the grid
 */ 
void Grid::placeSillyPad(int gridX, int gridY, int player) {
	sillyPads[gridX][gridY] = player;
	sillyPadsPlaced[gridX][gridY] = hge->Timer_GetTime();
}

/**
 * Places a super wall on the grid
 */
void Grid::placeSuperWall(int gridX, int gridY, int player) {
	superWalls[gridX][gridY] = player;
}

/**
 * Places a super flower on the grid
 */
void Grid::placeSuperFlower(int gridX, int gridY, int player) {
	superFlowers[gridX][gridY] = player;
}



