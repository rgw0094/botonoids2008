#include "Game.h"
#include "WallBreakerManager.h"
#include "MissileManager.h"

extern HGE *hge;
extern HCHANNEL musicChannel;
extern hgeResourceManager *resources;
extern GameInfo gameInfo;
extern Grid *grid;
extern GUI *gui;
extern Menu *menu;
extern MiniMenu *minimenu;
extern Player *players[3];
extern StatsPage *statsPage;
extern WallBreakerManager *wallBreakerManager;
extern MissileManager *missileManager;
extern ItemManager *itemManager;
extern hgeParticleManager *explosionManager;
extern hgeAnimation *botonoidGraphics[NUM_BOTONOIDS];
extern int mode;
extern float timer;
extern float countDownTimer;
extern int countDown;
extern bool menuMusicPlaying;
extern Song songs[NUM_SONGS];

/**
 * Returns the color that comes after currentColor
 */
int nextColor(int currentColor) {
	return (currentColor == PURPLE ? RED : currentColor + 1);
}

/**
 * Changes the music channel to play the specified song id.
 */
void setMusic(int music) {
	setMusic(songs[music].fileName);
}

/**
 * Changes the music channel to play the specified song name.
 */
void setMusic(char *music) {
	menuMusicPlaying = (strcmp(music, "menu") == 0);
	hge->Channel_Stop(musicChannel);
	hge->Music_SetPos(resources->GetMusic(music),0,0);
	gameInfo.currentMusic = resources->GetMusic(music);
	musicChannel = hge->Music_Play(gameInfo.currentMusic, true, gameInfo.musicVolume);
}

void stopMusic() {
	hge->Channel_Stop(musicChannel);
	menuMusicPlaying = false;
}

/**
 * Draw a collision box with the specified RGB values.
 */
void drawCollisionBox(hgeRect *box, int r, int g, int b) {

	hge->Gfx_RenderLine(box->x1, box->y1, box->x2, box->y1, ARGB(255,r,g,b));
	hge->Gfx_RenderLine(box->x2, box->y1, box->x2, box->y2, ARGB(255,r,g,b));
	hge->Gfx_RenderLine(box->x2, box->y2, box->x1, box->y2, ARGB(255,r,g,b));
	hge->Gfx_RenderLine(box->x1, box->y2, box->x1, box->y1, ARGB(255,r,g,b));

}

/**
 * This is called when the "Start Game" button is pressed in the Customize screen. 
 * The game is initialized based on the choices made in the menu which are held in 
 * the GameInfo struct.
 */
void startGame() {

	//Enter game state
	mode = GAME_MODE;
	countDown = 1;//4;
	countDownTimer = 0.0;//1.0
	stopMusic();

	//Set board size and time limit
	int distFromEdge;
	if (gameInfo.boardSize == CLASSIC) {
		gameInfo.timeLimit = 300.0;
		grid = new Grid(24,17);
		distFromEdge = 5;
	} else if (gameInfo.boardSize == LARGE) {
		gameInfo.timeLimit = 300.0;
		grid = new Grid(25,22);
		distFromEdge = 5;
	} else if (gameInfo.boardSize == MEDIUM) {
		gameInfo.timeLimit = 240.0;
		grid = new Grid(21,19);
		distFromEdge = 4;
	} else if (gameInfo.boardSize == SMALL) {
		gameInfo.timeLimit = 180.0;
		grid = new Grid(17,16);
		distFromEdge = 4;
	}

	//Create new players
	if (gameInfo.numPlayers >= 1) 
		players[0] = new Player(0 + distFromEdge, 0 + distFromEdge, 0, gameInfo.selectedBotonoid[0]);
	if (gameInfo.numPlayers >= 2) 
		players[1] = new Player(grid->width - distFromEdge - 1, 0 + distFromEdge, 1 , gameInfo.selectedBotonoid[1]);
	if (gameInfo.numPlayers >= 3) 
		players[2] = new Player(grid->width/2, grid->height - distFromEdge - 1, 2, gameInfo.selectedBotonoid[2]);

	//Reset game info
	timer = gameInfo.timeLimit;
	gameInfo.winner = -1;

	//Create new game objects
	if (gui) delete gui;
	gui = new GUI();
	if (statsPage) delete statsPage;
	statsPage = new StatsPage();
	if (itemManager) delete itemManager;
	itemManager = new ItemManager();
	if (wallBreakerManager) delete wallBreakerManager;
	wallBreakerManager = new WallBreakerManager();
	if (missileManager) delete missileManager;
	missileManager = new MissileManager();
	if (explosionManager) delete explosionManager;
	explosionManager = new hgeParticleManager();

	for (int i = 0; i < 3; i++) {
		botonoidGraphics[i]->SetSpeed(20);
	}

}

/**
 * Called when the timer reaches 0.
 */
void endGame() {

	timer = 0.0f;
	statsPage->active = true;

	

	//Play victory music
	//...

}

/**
 * Returns the grid coordinate of the pixel coordinate x.
 */
int getGridX(float x) {
	return (x-grid->xOffset) / 33;
}

/**
 * Returns the grid coordinate of the pixel coordinate y.
 */
int getGridY(float y) {
	return (y-grid->yOffset) / 33;
}

/**
 * Returns a time in seconds in MM:SS format
 */
std::string formatTime(int s) {

	//Create seconds string
	int seconds = s % 60;
	char secString[3];
	itoa(seconds, secString, 10);
	std::string secondsString = "";
	if (seconds < 10) {
		secondsString = "0";
		secondsString += secString;
	} else {
		secondsString = secString;
	}

	//Create minutes string
	int minutes = (s - seconds) / 60;
	char minString[3];
	itoa(minutes,minString, 10);

	//Combine minutes and seconds
	std::string timeString = "";
	timeString += minString;
	timeString += ":";
	timeString += secondsString;

	return timeString;

}

/**
 * Returns the maximum of 3 ints
 */
int maxInt(int num1, int num2, int num3) {

	if (num1 > num2 && num1 > num3) {
		return num1;
	} else if (num2 > num1 && num2 > num3) {
		return num2;
	} else if (num3 > num1 && num3 > num2) {
		return num3;
	}

}

/**
 * Returns the minimum of 3 ints
 */
int minInt(int num1, int num2, int num3) {

	if (num1 < num2 && num1 < num3) {
		return num1;
	} else if (num2 < num1 && num2 < num3) {
		return num2;
	} else if (num3 < num1 && num3 < num2) {
		return num3;
	}

}

/**
 * Returns the maximum of 4 floats
 */
int minFloat(float num1, float num2, float num3, float num4) {
	if (num1 < num2 & num1 < num3 && num1 < num4) {
		return num1;
	} else if (num2 < num1 && num2 < num3 && num2 < num4) {
		return num2;
	} else if (num3 < num1 && num3 < num2 && num3 < num4) {
		return num3;
	} else if (num4 < num1 && num4 < num2 && num4 < num3) {
		return num4;
	}
}

/**
 * Loads the previously saved item frequencies from the INI file
 */
void loadItemFrequencies() {
	std::string param;
	char num[5];
	for (int i = 0; i < 10; i++) {
		param = "itemFreq";
		param += itoa(i, num, 10);
		gameInfo.itemFrequencies[i] = hge->Ini_GetInt("Options", param.c_str(), 5);
	}
}

/**
 * Saves currently set item frequencies to the INI file
 */
void saveItemFrequencies() {
	std::string param;
	char num[5];
	for (int i = 0; i < 10; i++) {
		param = "itemFreq";
		param += itoa(i, num, 10);
		hge->Ini_SetInt("Options", param.c_str(), gameInfo.itemFrequencies[i]);
	}
}

/**
 * Returns the specified integer as a string because for the designers of C were too 
 * distracted by their beards to write a lanague that doESNT SUCK ASS FUCK SHIT
 */
char *intToString(int dickens) {
	char * intString = (char*)malloc(10);
	itoa(dickens, intString,10);
	return intString;
}

/**
 * Returns the distance between 2 points
 */
float dist(float x1, float y1, float x2, float y2) {
	return sqrt(pow(abs(x1 - x2), 2) + pow(abs(y1 - y2), 2));
}

/**
 * Returns whether or not point (x,y) is in bounds.
 */
bool isInBounds(float x, float y) {
	return (x > grid->xOffset &&
			x < grid->xOffset + 33.0 * grid->width &&
		    y > grid->yOffset &&
			y < grid->yOffset + 33.0 * grid->height);
}

/**
 * Creates an explosion at point (x,y) and plays a random explosion sound effect.
 */
void createExplosionAt(float x, float y) {
	//Play a random explosion sound
	explosionManager->SpawnPS(&resources->GetParticleSystem("explosion")->info, x, y);
	if (rand() % 1000 < 500) hge->Effect_Play(resources->GetEffect("snd_explosion1"));
	else hge->Effect_Play(resources->GetEffect("snd_explosion2"));
}

/**
 * Returns the distance between 2 points
 */
float distance(int x1, int y1, int x2, int y2) {
	return sqrt(float((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1)));
}
