#include "game.h"

HGE *hge=0;
Grid *grid;
hgeResourceManager *resources;
Player *players[3];
GUI *gui;
hgeFont *debugFont;
Menu *menu;
MiniMenu *minimenu;
Input *input;
StatsPage *statsPage;
ItemManager *itemManager;
std::string botonoidNames[3];

//Sounds
HCHANNEL musicChannel;

//Textures
HTEXTURE botonoidsTexture, tilesTexture;

//Sprites
hgeAnimation *botonoidGraphics[NUM_BOTONOIDS];
hgeSprite *specialTiles[3*4], *tileSprites[NUM_COLORS];

//Variables
GameInfo gameInfo;
int debug;
float timer;
int mode = MENU_MODE;

void loadResources() {

	//Create resource manager from resource script
	resources = new hgeResourceManager("Data/resources.res");
	hge->Resource_AttachPack("Data/Sounds.dat");

	//Botonoid names
	botonoidNames[0] = "Alphanoid";
	botonoidNames[1] = "Barvinoid";
	botonoidNames[2] = "Herbanoid";

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

	//Temp
	if (hge->Input_KeyDown(HGEK_T)) timer = 1.0f;

	//Always update input
	input->UpdateInput();

	//Update menu
	if (mode == MENU_MODE) {
		
		if (menu->update(dt)) return true;	

		//Press ESC to exit
		if (hge->Input_KeyDown(HGEK_ESCAPE)) return true;

	//Update game
	} else if (mode == GAME_MODE) {	

		//If the mini menu is active, only update it
		if (minimenu->active) {
			minimenu->update(dt);
			
		//If the stats page is active only update it
		} else if (statsPage->active) {
			statsPage->update(dt);

		} else {

			grid->update(dt);
			for (int i = 0; i < gameInfo.numPlayers; i++) players[i]->update(dt);
			gui->update(dt);
			itemManager->update(dt);

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
		for (int i = 0; i < gameInfo.numPlayers; i++) players[i]->draw(dt);
		gui->draw(dt);
		itemManager->draw(dt);
		minimenu->draw(dt);
		statsPage->draw(dt);

	}

	resources->GetFont("timer")->printf(5,5,HGETEXT_LEFT, "%d", gameInfo.winner);

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

		srand(time(NULL));
			
		//Load shit
		loadResources();
		
		//Defaults
		gameInfo.timeLimit = 300.0f;
		gameInfo.musicVolume = 100.0f;
		gameInfo.soundVolume = 100.0f;

		//Create Game Objects
		menu = new Menu();
		minimenu = new MiniMenu();	
		input = new Input();

		debugFont = new hgeFont("Data/Fonts/debug.fnt");

		//Let's rock now!
		hge->System_Start();

		//Free shit
		input->saveInputs();
		deleteResources();
		if (input) delete input;
		if (itemManager) delete itemManager;

	}
	else MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);

	// Clean up and shutdown
	hge->System_Shutdown();
	hge->Release();
	return 0;
}
