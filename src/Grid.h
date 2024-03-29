#ifndef _GRID_H_
#define _GRID_H_

#include "include/hge.h"
#include "include/hgeresource.h"

#define GARDEN_FADE_IN_TIME 0.5
#define GARDEN_FADE_OUT_TIME 0.5
#define GARDEN_FADE_OUT_DELAY 0.2

class Grid {

public:

	Grid(int height, int width);
	~Grid();

	//methods
	void draw(float dt);
	void update(float dt);
	void randomize();
	bool startColorChangeAt(int gridX, int gridY, int player);
	void doColorChanges(float dt);
	void resetVisited();
	void resetWallsTouched();
	int floodCount(int x, int y, int color);
	int getColorAt(int x, int y);
	bool buildWall(int x, int y, int player);
	void clearFoundations(int player);
	void countReachableWalls(int x, int y, int player);
	void fillGarden(int x, int y, int player);
	void unFillGarden(int x, int y);
	int numWallsTouched();	
	int countVisited();
	bool isWallAt(float x, float y);
	bool isOtherPlayersWallAt(int x, int y, int player);
	bool inBounds(float x, float y);
	int numFoundations(int player);
	bool placeSillyPad(int gridX, int gridY, int player);
	bool placeSuperWall(int gridX, int gridY, int player);
	bool placeSuperFlower(int gridX, int gridY, int player);
	void breakWallAt(int x, int y);

	//Variables
	int height, width, xOffset, yOffset;
	int gardenStartX, gardenStartY;

	int tiles[32][32];
	float startedColorChange[32][32];
	float alpha[32][32];
	bool visited[32][32];			//Used for recursion
	int foundations[32][32];		//Which player foundations belong to
	int walls[32][32];				//Which player walls belong to
	int gardens[32][32];			//Which player gardens belong to
	int sillyPads[32][32];			//Owner of silly pad
	float sillyPadsPlaced[32][32];	//Time silly pad placed
	int superWalls[32][32];
	int superFlowers[32][32];
	float timeToFadeGarden[32][32];
	float timeGardenCreated[32][32];
	bool wallsTouched[4];			//Which walls have been touched in the garden
									// finding algorithm.


	//Objects
	HGE *hge;
	hgeParticleManager *dustClouds;
	hgeSprite *sillyPadSprites[3], *superFlowerSprites[3], *superWallSprites[3], *tileSprites[5], *specialTiles[3*4];

};

#endif