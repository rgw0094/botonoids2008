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
		itemSprites[i] = new hgeSprite(resources->GetTexture("items"), 0, 32*i, 32, 32);
	}
}

/**
 * Destructor.
 */
ItemManager::~ItemManager() {
	for (int i = 0; i < NUM_ITEMS; i++) {
		delete itemSprites[i];
	}
	reset();
}

/**
 * Draw all managed items.
 */
void ItemManager::draw(float dt) {

	std::list<Item>::iterator i;
	for (i = itemList.begin(); i != itemList.end(); i++) {
		i->trail->Render();
		i->animation->Render(i->x, i->y);
	}

}

/**
 * Update all managed items.
 */
void ItemManager::update(float dt) {

	//Loop through each item
	std::list<Item>::iterator i;
	for (i = itemList.begin(); i != itemList.end(); i++) {

		//Update stuff
		i->y += i->dy*dt;
		i->x += i->dx*dt;
		i->trail->MoveTo(i->x, i->y);
		i->trail->Update(dt);
		i->collisionBox->SetRadius(i->x, i->y, i->radius);
		i->animation->Update(dt);

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
		bool itemCollected = false;
		for (int player = 0; player < gameInfo.numPlayers; player++) {
			if (players[player]->collisionBox->Intersect(i->collisionBox)) {
	
				//Player attempts to collects this item. If the player has no room in
				//his item wheel then the item can not be collected!!!!!!!
				itemCollected = players[player]->addItem(i->itemCode);
	
			}
		}

		//If the player collected the item, delete it
		if (itemCollected) {
			delete i->collisionBox;
			delete i->animation;
			i = itemList.erase(i);
		}

	}

}

/**
 * Generates a new random item. The bigger the garden the better the items!!!
 */
void ItemManager::generateItem(int gridX, int gridY, int gardenSize) {
		
	//Gardens must be at least 3 
	if (gardenSize < 2) return;

	//Determine what item to generate
	int item = rand() % 10;

	//Create the new item
	Item newItem;
	newItem.itemCode = item;
	newItem.radius = 13.0f;
	newItem.animation = new hgeAnimation(resources->GetTexture("items"),
											 32,		//frames
											 32,		//FPS
											 0,			//x
											 item*32,	//y
											 32.0f,		//w
											 32.0f);	//h
	newItem.animation->SetMode(HGEANIM_FWD | HGEANIM_LOOP);
	newItem.animation->SetHotSpot(16.0f, 16.0f);
	newItem.animation->Play();

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
	newItem.trail->FireAt(newItem.x, newItem.y);

	//Generate random initial direction
	int degrees = rand() % 360;
	float radians = (float)degrees * (PI / 180.0f);
	newItem.dx = 100.0f * cos(radians);
	newItem.dy = 100.0f * sin(radians);

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