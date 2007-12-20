#ifndef _GAME_H_
#define _GAME_H_

#include "time.h"
#include "include/hge.h"
#include "include/hgeresource.h"
#include "include/hgesprite.h"
#include "Grid.h"
#include "input.h"
#include "player.h"
#include "gui.h"
#include "menu.h"
#include "minimenu.h"
#include "statspage.h"
#include "itemmanager.h"
#include "button.h"
#include <string>

//Game modes
#define MENU_MODE 0
#define GAME_MODE 1

#define NUM_PLAYER_GRAPHICS 4		//Number of graphics per player
#define NUM_BOTONOIDS 3
#define PLAYER_1 0
#define PLAYER_2 1
#define DOWN 0
#define LEFT 1
#define UP 2
#define RIGHT 3
#define GRID_SIZE 32
#define COLOR_CHANGE_REFRESH 3.0f	//Time it takes for color change ability to refresh
#define COLOR_CHANGE_TIME 2.0f		//Time it takes a square to change colors

#define BUTTON_HEIGHT 71.0f
#define BUTTON_WIDTH 248.0f

//Colors
#define NUM_COLORS 5
#define RED 0
#define ORANGE 1
#define GREEN 2
#define BLUE 3
#define PURPLE 4

struct GameInfo {
	int numPlayers;
	int musicVolume;
	int soundVolume;
	int winner;
	float timeLimit;
};


//Global functions
int nextColor(int currentColor);
void setMusic(char *music);
void drawCollisionBox(hgeRect *box, int r, int g, int b);
int getGridX(float x);
int getGridY(float y);
void startGame();
void endGame();
std::string formatTime(int seconds);
int maxInt(int num1, int num2, int num3);
int minInt(int num1, int num2, int num3);

#endif