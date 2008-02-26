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
#include <string>

//Game modes
#define MENU_MODE 0
#define GAME_MODE 1

#define NUM_PLAYER_GRAPHICS 4		//Number of graphics per player
#define NUM_BOTONOIDS 3
#define ALPHANOID 0
#define BARVINOID 1
#define HERBANOID 2
#define PLAYER_1 0
#define PLAYER_2 1
#define DOWN 0
#define LEFT 1
#define UP 2
#define RIGHT 3
#define GRID_SIZE 32
#define COLOR_CHANGE_REFRESH 3.0f	//Time it takes for color change ability to refresh
#define COLOR_CHANGE_TIME 2.0f		//Time it takes a square to change colors
#define SILLY_PAD_DURATION 30.0f

#define BUTTON_HEIGHT 71.0f
#define BUTTON_WIDTH 248.0f

//Colors
#define NUM_COLORS 5
#define RED 0
#define ORANGE 1
#define GREEN 2
#define BLUE 3
#define PURPLE 4

//Board size
#define SMALL 0
#define MEDIUM 1
#define LARGE 2
#define CLASSIC 3

//Selectable game music
#define NUM_SONGS 13
struct Song {
	char songName[50];
	char fileName[50];
};

struct GameInfo {
	int numPlayers;
	int musicVolume;
	int soundVolume;
	int gameMusic;
	int winner;
	float timeLimit;
	int selectedBotonoid[3];
	int boardSize;
	int itemFrequencies[10];
	int pointsPerWall[3];
	int pointsPerGarden[3];
	HMUSIC currentMusic;
};

//Global functions
int nextColor(int currentColor);
void setMusic(int music);
void setMusic(char *music);
void stopMusic();
void drawCollisionBox(hgeRect *box, int r, int g, int b);
int getGridX(float x);
int getGridY(float y);
void startGame();
void endGame();
std::string formatTime(int seconds);
int maxInt(int num1, int num2, int num3);
int minFloat(float num1, float num2, float num3, float num4);
int minInt(int num1, int num2, int num3);
void loadItemFrequencies();
void saveItemFrequencies();
float dist(float x1, float y1, float x2, float y2);
char* intToString(int num);
bool isInBounds(float x, float y);
void createExplosionAt(float x, float y);
float distance(int x1, int y1, int x2, int y2);

#endif