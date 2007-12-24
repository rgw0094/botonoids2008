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

		itemAnimations[i] = new hgeAnimation(resources->GetTexture("items"),	//texture
											 16, //frames
											 16, //FPS
											 0,	 //x
											 0,	 //y
											 32.0f, //w
											 32.0f); //h
		itemAnimations[i]->SetMode(HGEANIM_FWD | HGEANIM_LOOP);
		itemAnimations[i]->Play();

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
		i->trail->MoveTo(i->x + 16.0f, i->y + 16.0f);
		i->trail->Update(dt);
		i->trail->Render();
		itemAnimations[i->itemCode]->Render(i->x, i->y);
	}

}

/**
 * Update all managed items.
 */
void ItemManager::update(float dt) {

	//Update item animations
	for (int i = 0; i < NUM_ITEMS; i++) {
		itemAnimations[i]->Update(dt);
	}

	//Loop through each item
	std::list<Item>::iterator i;
	for (i = itemList.begin(); i != itemList.end(); i++) {

		//Update position
		i->y += i->dy*dt;
		i->x += i->dx*dt;
		i->collisionBox->SetRadius(i->x, i->y, i->radius);

		float xDist = 3.0f * (dt < 0.01f ? 0.01f : dt) * i->dx + i->radius * (i->dx > 0.0f ? 1.0f : -1.0f);
		float yDist = 3.0f * (dt < 0.01f ? 0.01f : dt) * i->dy + i->radius * (i->dy > 0.0f ? 1.0f : -1.0f);

		//Horizontal collision
		if (grid->isWallAt(i->x + xDist, i->y) || !grid->inBounds(i->x + xDist, i->y)) {
			i->dx = -i->dx;
		}

		//Vertical collision
		if (grid->isWallAt(i->x, i->y + yDist) || !grid->inBounds(i->x, i->y + yDist)){
			i->dy = -i->dy;
		}

		//Check for collision with player
		bool collided = false;
		for (int player = 0; player < gameInfo.numPlayers; player++) {
			if (players[player]->collisionBox->Intersect(i->collisionBox) && i->player == player) {
	
				//Player collects this item
				players[player]->addItem(i->itemCode);
				collided = true;
	
			}
		}
		if (collided) {
			//Remove the item from the list
			delete i->collisionBox;
			i = itemList.erase(i);
		}

	}

}

/**
 * Generates a new random item. The bigger the garden the better the items!!!
 */
void ItemManager::generateItem(int gridX, int gridY, int gardenSize, int whichPlayer) {
		
	//Gardens must be at least 3 
	if (gardenSize < 2) return;

	//Determine what item to generate
	int item = ITEM_SILLY_PAD;

	//Create the new item
	Item newItem;
	newItem.itemCode = item;
	newItem.player = whichPlayer;
	newItem.radius = 13.0f;

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

	//Set collision box
	newItem.collisionBox = new hgeRect();
	newItem.collisionBox->SetRadius(newItem.x, newItem.y, 12.0f);

	//Create particle trail
	newItem.trail = new hgeParticleSystem("Data/particle9.psi", resources->GetSprite("particleGraphic5"));
	newItem.trail->FireAt(newItem.x+16.0f, newItem.y+16.0f);

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
		delete i->trail;
		delete i->collisionBox;
		i = itemList.erase(i);
	}
}