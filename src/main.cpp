#include "game.h"
#include "WallBreakerManager.h"
#include "MissileManager.h"
#using "mscorlib.dll"

HGE *hge=0;
Grid *grid;
hgeResourceManager *resources;
Player *players[3];
GUI *gui;
Menu *menu;
MiniMenu *minimenu;
Input *input;
StatsPage *statsPage;
ItemManager *itemManager;
WallBreakerManager *wallBreakerManager;
MissileManager *missileManager;
hgeParticleManager *explosionManager;

std::string botonoidNames[3];
Song songs[NUM_SONGS];
hgeAnimation *itemAnimations[10];

//Sounds
HCHANNEL musicChannel;

//Textures
HTEXTURE botonoidsTexture, tilesTexture;

//Sprites
hgeAnimation *botonoidGraphics[NUM_BOTONOIDS];
hgeSprite *specialTiles[3*4], *tileSprites[NUM_COLORS], *particleSprites[16];

//Variables
GameInfo gameInfo;
int debug;
float timer;
int countDown;
float countDownTimer;
int mode = MENU_MODE;
float gameTime;
bool menuMusicPlaying;
bool debugMode;

void loadResources() {

	//Create resource manager from resource script
	resources = new hgeResourceManager("Data/resources.res");
	hge->Resource_AttachPack("Data/Sounds.zip");

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

	//Load particle sprites
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			particleSprites[i*4+j] = new hgeSprite(resources->GetTexture("particleTex"), i*32, j*32, 32, 32);
			particleSprites[i*4+j]->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE);
			particleSprites[i*4+j]->SetHotSpot(16,16);
		}
	}

	//Load item animations
	for (int i = 0; i < 10; i++) {
		itemAnimations[i] = new hgeAnimation(resources->GetTexture("items"),
											 32,		//frames
											 20,		//FPS
											 0,			//x
											 i*32,		//y
											 32.0f,		//w
											 32.0f);	//h
		itemAnimations[i]->SetMode(HGEANIM_FWD | HGEANIM_LOOP);
		itemAnimations[i]->SetHotSpot(16.0f, 16.0f);
		itemAnimations[i]->Play();
	}

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

	//Selectable songs
	strcpy(songs[0].fileName, "stStyle");
	strcpy(songs[0].songName, "ST-Style");
	strcpy(songs[1].fileName, "mtalking");
	strcpy(songs[1].songName, "Modern Talking");
	strcpy(songs[2].fileName, "her11");
	strcpy(songs[2].songName, "Her 11");
	strcpy(songs[3].fileName, "trainer5");
	strcpy(songs[3].songName, "Trainer 5");
	strcpy(songs[4].fileName, "heidi7");
	strcpy(songs[4].songName, "Heidi #7");
	strcpy(songs[5].fileName, "aSadTouch");
	strcpy(songs[5].songName, "A Sad Touch");
	strcpy(songs[6].fileName, "noMercy");
	strcpy(songs[6].songName, "No Mercy");
	strcpy(songs[7].fileName, "saturn");
	strcpy(songs[7].songName, "Saturn");
	strcpy(songs[8].fileName, "moskito");
	strcpy(songs[8].songName, "Moskito");
	strcpy(songs[9].fileName, "jttimarsuEdit");
	strcpy(songs[9].songName, "Jttimarsu Edit");
	strcpy(songs[10].fileName, "demag");
	strcpy(songs[10].songName, "Demag");
	strcpy(songs[11].fileName, "sierain2");
	strcpy(songs[11].songName, "Sierain 2");
	strcpy(songs[12].fileName, "puzzler");
	strcpy(songs[12].songName, "Classic Puzzler");
	
	//Botonoid names
	botonoidNames[0] = "Alphanoid";
	botonoidNames[1] = "Barvinoid";
	botonoidNames[2] = "Herbonoid";

}

bool FrameFunc() {

	float dt=hge->Timer_GetDelta();

	//Always update input
	input->UpdateInput();

	//Toggle debug mode
	if (hge->Input_KeyDown(HGEK_D)) debugMode = !debugMode;
	if (hge->Input_KeyDown(HGEK_T)) timer = 1.0f;

	//Update menu
	if (mode == MENU_MODE) {
		
		if (menu->update(dt)) return true;	

		//Press ESC to exit
		if (hge->Input_KeyDown(HGEK_ESCAPE)) return true;

	//Update game
	} else if (mode == GAME_MODE) {	

		//Count down until start
		if (countDown > 0) {
			countDownTimer -= dt;
			if (countDownTimer < 0.0f) {
				//Decrease counter
				countDown--;
				countDownTimer = 1.0f;
				if (countDown == 0) {
					//If this was the final count down (GOOD SONG) start the music
					setMusic(gameInfo.gameMusic);
				} else {
					hge->Effect_Play(resources->GetEffect("snd_countdown"));
				}
			}

		//If the mini menu is active, only update it
		} else if (minimenu->active) {
			minimenu->update(dt);
				
		//If the stats page is active only update it
		} else if (statsPage->active) {
			statsPage->update(dt);

		} else {

			gameTime += dt;
			grid->update(dt);
			for (int i = 0; i < gameInfo.numPlayers; i++) players[i]->update(dt);
			gui->update(dt);
			itemManager->update(dt);
			wallBreakerManager->update(dt);
			missileManager->update(dt);
			explosionManager->Update(dt);

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
		gui->draw(dt);
		wallBreakerManager->draw(dt);
		for (int i = 0; i < gameInfo.numPlayers; i++) players[i]->draw(dt);
		missileManager->draw(dt);
		itemManager->draw(dt);
		explosionManager->Render();
		minimenu->draw(dt);
		statsPage->draw(dt);
	
		//Draw countdown
		if (countDown == 3) resources->GetSprite("countdown3")->Render(400,300);
		if (countDown == 2) resources->GetSprite("countdown2")->Render(400,300);
		if (countDown == 1) resources->GetSprite("countdown1")->Render(400,300);

	}

	if (debugMode) {
		resources->GetFont("timer")->printf(5,5,HGETEXT_LEFT, "%f", 1.0/dt);
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

		try {

			srand(time(NULL));
			gameTime = 0.0f;	
			debugMode = false;

			//Load shit
			loadResources();
			
			//Settings
			gameInfo.timeLimit = 300.0f;
			gameInfo.musicVolume = hge->Ini_GetInt("Options", "musicVolume", 100);
			gameInfo.soundVolume = hge->Ini_GetInt("Options", "soundVolume", 100);
			gameInfo.boardSize = LARGE;
			loadItemFrequencies();

			//Create Game Objects
			menu = new Menu();
			minimenu = new MiniMenu();	
			input = new Input();

			//Start menu music
			setMusic("menu");

			hge->System_Start();

		} catch(System::Exception *ex) {
			MessageBox(NULL, "A fatal error has occurred and the program must exit. \nCheck Smiley.log for more information. \nIt sure would be nice to display the message here but C++ sucks ass", "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
			hge->System_Log("----FATAL ERROR----------");
			hge->System_Log("%s", ex->ToString());
			exit(1);
		}

		//Free stuff
		input->saveInputs();
		
	}
	else MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);

	// Clean up and shutdown
	hge->System_Shutdown();
	hge->Release();
	return 0;
}
