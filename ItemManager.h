#ifndef _ITEMMANAGER_H_
#define _ITEMMANAGER_H_

#include "game.h"

#define NUM_ITEMS 10
#define ITEM_SILLY_PAD 0
#define ITEM_WALLBREAKER 1
#define ITEM_HEALTH 2
#define ITEM_MISSILE 3
#define ITEM_NAPALM 4
#define ITEM_PUNCH_GLOVE 5
#define ITEM_SUPER_FLOWER 6
#define ITEM_SLOW 7
#define ITEM_SUPER_WALL 8
#define ITEM_COLOR_BOMB 9

struct Item {
	int itemCode;
	float x, y, dx, dy, radius;
	hgeParticleSystem *trail;
	hgeRect *collisionBox;
	hgeAnimation *animation;
};


class ItemManager {

public:

	ItemManager();
	~ItemManager();

	void draw(float dt);
	void update(float dt);
	void generateItem(int gridX, int gridY, int gardenSize);
	void reset();

	std::list<Item> itemList;
	hgeSprite *itemSprites[NUM_ITEMS];

};

#endif