#include "Game.h"

extern HGE *hge;
extern controls playerControls[3];
extern HCHANNEL musicChannel;
extern int musicVolume;
extern hgeResourceManager *resources;

/**
 * Load player controls from the .ini file.
 */ 
void loadControls() {

	playerControls[0].moveLeft = hge->Ini_GetInt("Controls", "p1Left", HGEK_LEFT);
	playerControls[0].moveRight = hge->Ini_GetInt("Controls", "p1Right", HGEK_RIGHT);
	playerControls[0].moveUp = hge->Ini_GetInt("Controls", "p1Up", HGEK_UP);
	playerControls[0].moveDown = hge->Ini_GetInt("Controls", "p1Down", HGEK_DOWN);
	playerControls[0].changeColor = hge->Ini_GetInt("Controls", "p1Change", HGEK_CTRL);

	playerControls[1].moveLeft = hge->Ini_GetInt("Controls", "p2Left", HGEK_A);
	playerControls[1].moveRight = hge->Ini_GetInt("Controls", "p2Right", HGEK_D);
	playerControls[1].moveUp = hge->Ini_GetInt("Controls", "p2Up", HGEK_W);
	playerControls[1].moveDown = hge->Ini_GetInt("Controls", "p2Down", HGEK_S);
	playerControls[1].changeColor = hge->Ini_GetInt("Controls", "p2Change", HGEK_SHIFT);

	playerControls[2].moveLeft = hge->Ini_GetInt("Controls", "p3Left", HGEK_NUMPAD4);
	playerControls[2].moveRight = hge->Ini_GetInt("Controls", "p3Right", HGEK_NUMPAD6);
	playerControls[2].moveUp = hge->Ini_GetInt("Controls", "p3Up", HGEK_NUMPAD8);
	playerControls[2].moveDown = hge->Ini_GetInt("Controls", "p3Down", HGEK_NUMPAD5);
	playerControls[2].changeColor = hge->Ini_GetInt("Controls", "p3Change", HGEK_NUMPAD1);

}

/**
 * Save player controls to the .ini file.
 */
void saveControls() {

	hge->Ini_SetInt("Controls","p1Left",playerControls[0].moveLeft);
	hge->Ini_SetInt("Controls","p1Right",playerControls[0].moveRight);
	hge->Ini_SetInt("Controls","p1Up",playerControls[0].moveUp);
	hge->Ini_SetInt("Controls","p1Down",playerControls[0].moveDown);

	hge->Ini_SetInt("Controls","p2Left",playerControls[1].moveLeft);
	hge->Ini_SetInt("Controls","p2Right",playerControls[1].moveRight);
	hge->Ini_SetInt("Controls","p2Up",playerControls[1].moveUp);
	hge->Ini_SetInt("Controls","p2Down",playerControls[1].moveDown);

	hge->Ini_SetInt("Controls","p3Left",playerControls[2].moveLeft);
	hge->Ini_SetInt("Controls","p3Right",playerControls[2].moveRight);
	hge->Ini_SetInt("Controls","p3Up",playerControls[2].moveUp);
	hge->Ini_SetInt("Controls","p3Down",playerControls[2].moveDown);

}

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
	musicChannel = hge->Music_Play(resources->GetMusic(music),true,musicVolume);
}