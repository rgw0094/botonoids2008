#include "itemmanager.h"
#include "grid.h"

extern HGE *hge;
extern hgeResourceManager *resources;
extern Grid *grid;
extern Player *players[3];
extern GameInfo gameInfo;

/**
 * Constructor.
 */
ItemManager::ItemManager() {
	//Load item sprites
	for (int i = 0; i < NUM_ITEMS; i++) {
		itemSprites[i] = new hgeSprite(resources->GetTexture("items"),i*32,0,32,32);
		itemSprites[i]->SetHotSpot(16,16);
	}
}

/**
 * Destructor.
 */
ItemManager::~ItemManager() {
	for (int i = 0; i < NUM_ITEMS; i++) delete itemSprites[i];
	reset();
}

/**
 * Draw all managed items.
 */
void ItemManager::draw(float dt) {

	std::list<Item>::iterator i;
	for (i = itemList.begin(); i != itemList.end(); i++) {
		itemSprites[i->itemCode]->Render(i->x, i->y);
	}

}

/**
 * Update all managed items.
 */
void ItemManager::update(float dt) {

	//Loop through each item
	std::list<Item>::iterator i;
	for (i = itemList.begin(); i != itemList.end(); i++) {

		//Update position
		i->y += i->dy*dt;
		i->x += i->dx*dt;

		float xDist = 3.0f * (dt < 0.01f ? 0.01f : dt) * i->dx;
		float yDist = 3.0f * (dt < 0.01f ? 0.01f : dt) * i->dy;

		//Horizontal collision
		if (grid->isWallAt(i->x + xDist, i->y) || !grid->inBounds(i->x + xDist, i->y)) {
			i->dx = -i->dx;
		}

		//Vertical collision
		if (grid->isWallAt(i->x, i->y + yDist) || !grid->inBounds(i->x, i->y + yDist)){
			i->dy = -i->dy;
		}

		//Check for collision with player
		for (int player = 0; player < gameInfo.numPlayers; player++) {
			if (players[player]->collisionBox->TestPoint(i->x, i->y)) {
				//Player collects this item

				//Remove the item from the list
				i = itemList.erase(i);
			}
		}

	}

}

/**
 * Generates a new random item. The bigger the garden the better the items!!!
 */
void ItemManager::generateItem(int gridX, int gridY, int gardenSize) {
		
	//Determine what item to generate
	int item = 1;

	//Create the new item
	Item newItem;
	newItem.itemCode = item;

	//Randomly choose a square marked by the grid->visited[][] array to spawn the item in.
	int randNum = rand() % 10000;
	int range = 10000 / gardenSize;
	int lowerBound;
	int upperBound;
	int count = 0;
	for (int i = 0; i < grid->width; i++) {
		for (int j = 0; j < grid->height; j++) {
			if (grid->visited[i][j]) {
				lowerBound = count*range;
				upperBound = lowerBound + range;
				if (randNum >= lowerBound && randNum < upperBound) {
					newItem.x = i*33+16 + grid->xOffset;
					newItem.y = j*33+16 + grid->yOffset;
				}
				count++;
			}
		}
	}

	//Generate random initial velocity
	int dir1 = rand() % 2;
	int dir2 = rand() % 2;
	newItem.dx = (rand() % 100 + 50) * (dir1 == 0 ? -1 : 1);
	newItem.dy = (rand() % 200) * (dir2 == 0 ? -1 : 1);

	//Add it to the item list
	itemList.push_back(newItem);

}

/**
 * Deletes all items
 */
void ItemManager::reset() {
	std::list<Item>::iterator i;
	for (i = itemList.begin(); i != itemList.end(); i++) {
		i = itemList.erase(i);
	}
}