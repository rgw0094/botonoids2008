#include "Game.h"

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
extern int mode;
extern float timer;

/**
 * Returns the color that comes after currentColor
 */
int nextColor(int currentColor) {
	return (currentColor == PURPLE ? RED : currentColor + 1);
}

/**
 * Changes the music channel to play the specified song.
 */
void setMusic(char *music) {
	hge->Channel_Stop(musicChannel);
	hge->Music_SetPos(resources->GetMusic(music),0,0);
	musicChannel = hge->Music_Play(resources->GetMusic(music), true, gameInfo.musicVolume);
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
 * This is called after the players have navigated the menu. Game objects are 
 * created based on the choices players made.
 */
void startGame() {

	//Enter game state
	mode = GAME_MODE;

	//Set selected music
	setMusic("song3");

	//Reset game info
	timer = gameInfo.timeLimit;
	gameInfo.winner = -1;

	//Create new game objects
	if (grid) delete grid;
	grid = new Grid(25,22);
	if (gui) delete gui;
	gui = new GUI();
	if (statsPage) delete statsPage;
	statsPage = new StatsPage();

	players[0] = new Player(5,5,0,0);
	players[1] = new Player(15,5,1,1);
	if (gameInfo.numPlayers == 3) players[2] = new Player(10,15,2,2);

}

/**
 * Called when the timer reaches 0.
 */
void endGame() {

	timer = 0.0f;
	statsPage->active = true;

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

	//Play victory music
	//...

}