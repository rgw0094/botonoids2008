#include "game.h"

HGE *hge=0;
Grid *grid;
hgeResourceManager *resources;
Player *players[3];
GUI *gui;
hgeFont *debugFont;
Menu *menu;
MiniMenu *minimenu;

//Sounds
HCHANNEL musicChannel;

//Textures
HTEXTURE botonoidsTexture, tilesTexture;

//Sprites
hgeAnimation *botonoidGraphics[NUM_BOTONOIDS];
hgeSprite *specialTiles[3*4], *tileSprites[NUM_COLORS];

//Variables
int numPlayers = 3;
controls playerControls[3];
int debug;
int musicVolume = 100;
float timer = 300.0f;
int mode = GAME_MODE;

void loadResources() {

	//Create resource manager from resource script
	resources = new hgeResourceManager("Data/resources.res");
	hge->Resource_AttachPack("Data/Sounds.dat");

	//Load textures
	botonoidsTexture = resources->GetTexture("botonoids");
	tilesTexture = resources->GetTexture("tiles");

	//Load botonoid sprites
	botonoidGraphics[0] = resources->GetAnimation("goldBotonoid");
	botonoidGraphics[0]->Play();
	botonoidGraphics[1] = resources->GetAnimation("blackBotonoid");
	botonoidGraphics[1]->Play();
	botonoidGraphics[2] = resources->GetAnimation("silverBotonoid");
	botonoidGraphics[2]->Play();

	//Load tile sprites
	for (int i = 0; i < NUM_COLORS; i++) {
		tileSprites[i] = new hgeSprite(tilesTexture,0,i*32,32,32);
	}
	//Load special tiles
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			specialTiles[i*4 + j] = new hgeSprite(tilesTexture,32 + 32*i, 32*j, 32,32);
		}
	}

}



void deleteResources() {
	delete resources;
}



bool FrameFunc() {

	float dt=hge->Timer_GetDelta();

	//Update menu
	if (mode == MENU_MODE) {
		menu->update(dt);	

		//Press ESC to exit
		if (hge->Input_KeyDown(HGEK_ESCAPE)) return true;

	//Update game
	} else if (mode == GAME_MODE) {	

		//If the mini menu is active, only update it
		if (minimenu->active) {
			minimenu->update(dt);
		} else {
			grid->update(dt);
			for (int i = 0; i < numPlayers; i++) players[i]->update(dt);
			gui->update(dt);

			//Press ESC to open exit/continue menu
			if (hge->Input_KeyDown(HGEK_ESCAPE)) minimenu->active = true;

		}

	}


	// Continue execution
	return false;
}


bool RenderFunc() {

	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0);
	float dt=hge->Timer_GetDelta();

	//Draw menu
	if (mode == MENU_MODE) {
		menu->draw(dt);

	//Draw game
	} else if (mode == GAME_MODE) {

		grid->draw(dt);
		for (int i = 0; i < numPlayers; i++) players[i]->draw(dt);
		gui->draw(dt);
		minimenu->draw(dt);

	}

	// End rendering and update the screen
	hge->Gfx_EndScene();
	return false;
}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// Get HGE interface
	hge = hgeCreate(HGE_VERSION);

	//Set system state
	hge->System_SetState(HGE_LOGFILE, "BotonoidsLog.txt");
	hge->System_SetState(HGE_INIFILE, "Data/Botonoids2008.ini");
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
	hge->System_SetState(HGE_TITLE, "Botonoids 2008");
	hge->System_SetState(HGE_WINDOWED, true);
	hge->System_SetState(HGE_SCREENWIDTH, 1024);
	hge->System_SetState(HGE_SCREENHEIGHT, 768);
	hge->System_SetState(HGE_SCREENBPP, 32);
	hge->System_SetState(HGE_SHOWSPLASH, false);

	if(hge->System_Initiate()) {

		//Load shit
		loadResources();
		loadControls();

		setMusic("song3");

		//Make objects
		grid = new Grid(25,22);
		players[0] = new Player(5,5,0,0);
		players[1] = new Player(15,5,1,1);
		if (numPlayers == 3) players[2] = new Player(10,15,2,2);
		gui = new GUI();
		menu = new Menu();
		minimenu = new MiniMenu();

		debugFont = new hgeFont("Data/Fonts/debug.fnt");

		//Let's rock now!
		hge->System_Start();

		//Free shit
		deleteResources();
		saveControls();

	}
	else MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);

	// Clean up and shutdown
	hge->System_Shutdown();
	hge->Release();
	return 0;
}
