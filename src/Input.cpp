#include "Input.h"

#include "game.h"
#include "input.h"

extern HGE *hge;

// Globals ////////////
LPDIRECTINPUT8       g_pDI;
LPDIRECTINPUTDEVICE8 g_pJoystick;
DIDEVCAPS            g_diDevCaps;

// Callback functions. They have to be globals in the cpp file for some reason /////////
BOOL CALLBACK    EnumAxesCallback( const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext );
BOOL CALLBACK    EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance, VOID* pContext );


/**
 * Constructor
 */
Input::Input() {

	g_pDI=NULL;
	g_pJoystick=NULL;
	
	//Initialize input states
	for (int player = 0; player < 3; player++) {
		for (int i = 0; i < NUM_INPUTS; i++) {
			inputs[player][i].pressed = inputs[player][i].prevPressed = false;
			inputs[player][i].whichDevice = DEVICE_KEYBOARD;
		}
	}

	//Load controls from .INI file
	loadInputs();

	//Set control names
	for (int i = 0; i < 3; i++) {
		strcpy(inputs[i][INPUT_LEFT].name, "Move Left");
		strcpy(inputs[i][INPUT_RIGHT].name, "Move Right");
		strcpy(inputs[i][INPUT_DOWN].name, "Move Down");
		strcpy(inputs[i][INPUT_UP].name, "Move Up");
		strcpy(inputs[i][INPUT_ACTION].name, "Attack/Select");
		strcpy(inputs[i][INPUT_ITEM].name, "Use Ability");
		strcpy(inputs[i][INPUT_NEXT_ITEM].name, "Next Ability");
		strcpy(inputs[i][INPUT_LAST_ITEM].name, "Previous Ability");
	}

	//Set edit modes to false
	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < NUM_INPUTS; i++) {
			inputs[j][i].editMode = false;	
		}
	}

	//Set game pad button states to unpressed
	for (int i = 0; i < 256; i++) gamePadButtons[i] = false;
	for (int i = 0; i < 4; i++) {
		joystickState[i] = false;
		previousJoystickState[i] = false;
	}

	//Initialize input - gets pointer to DirectInput interface and
	//registers the input device.
	InitInput(hge->System_GetState(HGE_HWND));

}

/**
 * Destructor
 */ 
Input::~Input() {
	FreeDirectInput();
}

//-----------------------------------------------------------------------------
// Name: InitInput()
// Desc: Acquires the gamepad
//-----------------------------------------------------------------------------
void Input::InitInput(HWND hDlg) { 
	acquiredJoystick = (FAILED(InitDirectInput(hDlg)) && FAILED(g_pJoystick->Acquire()));
}

//-----------------------------------------------------------------------------
// Name: UpdateInput()
// Desc: Updates the input
//-----------------------------------------------------------------------------
void Input::UpdateInput() {
	
	//Update previous states
	for (int player = 0; player < 3; player++) {
		for (int i = 0; i < NUM_INPUTS; i++) {
			inputs[player][i].prevPressed = inputs[player][i].pressed;
		}
	}

	HRESULT     hr;
	CHAR        strText[128]; // Device state text
	DIJOYSTATE2 js;           // DInput joystick state 
	CHAR*       str;

	if (NULL != g_pJoystick) {

		// Poll the device to read the current state
		hr = g_pJoystick->Poll(); 
		if( FAILED(hr) ) {
				// DInput is telling us that the input stream has been
				// interrupted. We aren't tracking any state between polls, so
				// we don't have any special reset that needs to be done. We
				// just re-acquire and try again.
				hr = g_pJoystick->Acquire();
				while( hr == DIERR_INPUTLOST ) 
					hr = g_pJoystick->Acquire();
				// hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
				// may occur when the app is minimized or in the process of 
				// switching, so just try again later 
				return; 
			}

		//Get the input's device state
		if(FAILED(hr = g_pJoystick->GetDeviceState( sizeof(DIJOYSTATE2), &js ))) return;

		//Save gamepad button states so they can be referenced later
		for (int i = 0; i < 256; i++) gamePadButtons[i] = js.rgbButtons[i];
		updateJoystickDirection(js);

	}

	//Update inputs for each player
	for (int player = 0; player < 3; player++) {

		//Update each input
		for (int i = 0; i < NUM_INPUTS; i++) {

			//Keyboard
			if (inputs[player][i].whichDevice == DEVICE_KEYBOARD) {
				inputs[player][i].pressed = hge->Input_GetKeyState(inputs[player][i].code);
				
			//Gamepad
			} else {
					
				int code = inputs[player][i].code;
					
				//Joystick directions are a special case because the joystick
				//is polled seperately from the gamepad buttons.
				if (code == JOYSTICK_LEFT) 
					inputs[player][i].pressed = (joystickDirection == INPUT_LEFT);
				else if (code == JOYSTICK_RIGHT) 
					inputs[player][i].pressed = (joystickDirection == INPUT_RIGHT);
				else if (code == JOYSTICK_UP)
					inputs[player][i].pressed = (joystickDirection == INPUT_UP);
				else if (code == JOYSTICK_DOWN)
					inputs[player][i].pressed = (joystickDirection == INPUT_DOWN);
				else
					inputs[player][i].pressed = gamePadButtons[inputs[player][i].code];

			}


		}

	}

}

//-----------------------------------------------------------------------------
// Name: InitDirectInput()
// Desc: Initialize the DirectInput variables.
//-----------------------------------------------------------------------------
HRESULT Input::InitDirectInput( HWND hDlg ) {
	
    HRESULT hr;

    // Register with the DirectInput subsystem and get a pointer
    // to a IDirectInput interface we can use.
    // Create a DInput object
    if( FAILED( hr = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
                                         IID_IDirectInput8, (VOID**)&g_pDI, NULL ) ) )
        return hr;

    // Look for a simple joystick we can use for this sample program.
	if( FAILED( hr = g_pDI->EnumDevices( DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback,
                                         NULL, DIEDFL_ATTACHEDONLY ) ) )
        return hr;

    // Make sure we got a joystick
	if( NULL == g_pJoystick ) {
        return S_OK;
	}

    // Set the data format to "simple joystick" - a predefined data format 
    //
    // A data format specifies which controls on a device we are interested in,
    // and how they should be reported. This tells DInput that we will be
    // passing a DIJOYSTATE2 structure to IDirectInputDevice::GetDeviceState().
    if( FAILED( hr = g_pJoystick->SetDataFormat( &c_dfDIJoystick2 ) ) )
		return hr;

    // Set the cooperative level to let DInput know how this device should
    // interact with the system and with other DInput applications.
    if( FAILED( hr = g_pJoystick->SetCooperativeLevel( hDlg, DISCL_EXCLUSIVE | 
                                                             DISCL_FOREGROUND ) ) )
        return hr;

    // Determine how many axis the joystick has (so we don't error out setting
    // properties for unavailable axis)
    g_diDevCaps.dwSize = sizeof(DIDEVCAPS);
    if ( FAILED( hr = g_pJoystick->GetCapabilities(&g_diDevCaps) ) )
        return hr;

    // Enumerate the axes of the joyctick and set the range of each axis. Note:
    // we could just use the defaults, but we're just trying to show an example
    // of enumerating device objects (axes, buttons, etc.).
	if ( FAILED( hr = g_pJoystick->EnumObjects( EnumAxesCallback, 
                                                NULL, DIDFT_AXIS ) ) )
        return hr;

	

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: EnumJoysticksCallback()
// Desc: Called once for each enumerated joystick. If we find one, create a
//       device interface on it so we can play with it.
//-----------------------------------------------------------------------------
BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance, VOID* pContext ) {

    HRESULT hr;

    // Obtain an interface to the enumerated joystick.
    hr = g_pDI->CreateDevice( pdidInstance->guidInstance, &g_pJoystick, NULL );

    // If it failed, then we can't use this joystick. (Maybe the user unplugged
    // it while we were in the middle of enumerating it.)
    if( FAILED(hr) ) 
        return DIENUM_CONTINUE;

    // Stop enumeration. Note: we're just taking the first joystick we get. You
    // could store all the enumerated joysticks and let the user pick.
    return DIENUM_STOP;
}

//-----------------------------------------------------------------------------
// Name: FreeDirectInput()
// Desc: Release the DirectInput variables.
//-----------------------------------------------------------------------------
VOID Input::FreeDirectInput() {
    // Unacquire the device one last time just in case 
    // the app tried to exit while the device is still acquired.
    if( g_pJoystick ) 
        g_pJoystick->Unacquire();
    
    // Release any DirectInput objects.
    SAFE_RELEASE( g_pJoystick );
    SAFE_RELEASE( g_pDI );
}

//-----------------------------------------------------------------------------
// Name: EnumAxesCallback()
// Desc: Callback function for enumerating the axes on a joystick
//-----------------------------------------------------------------------------
BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext ) {

    HWND hDlg = (HWND)pContext;

    DIPROPRANGE diprg; 
    diprg.diph.dwSize       = sizeof(DIPROPRANGE); 
    diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
    diprg.diph.dwHow        = DIPH_BYID; 
    diprg.diph.dwObj        = pdidoi->dwType; // Specify the enumerated axis
    diprg.lMin              = -1000; 
    diprg.lMax              = +1000; 
	
	// Set the range for the axis
	if( FAILED( g_pJoystick->SetProperty( DIPROP_RANGE, &diprg.diph ) ) ) {
		return DIENUM_STOP;
	}

    return DIENUM_CONTINUE;
}

/**
 * Returns whether or not the specified button is currently pressed.
 */
bool Input::buttonDown(int key, int player) {
	return inputs[player][key].pressed;
}

/**
 * Desc: Returns whether or not the specified key was pressed this frame.
 */
bool Input::buttonPressed(int key, int player) {
	return (inputs[player][key].pressed && !inputs[player][key].prevPressed);
}

//-----------------------------------------------------------------------------
// Name: saveInputs()
// Desc: Saves the input values to Botonoids2008.ini
//-----------------------------------------------------------------------------
void Input::saveInputs() {

	//Player 1
	hge->Ini_SetInt("Controls", "p1Left", inputs[0][INPUT_LEFT].code);
	hge->Ini_SetInt("Controls", "p1Right", inputs[0][INPUT_RIGHT].code);
	hge->Ini_SetInt("Controls", "p1Up", inputs[0][INPUT_UP].code);
	hge->Ini_SetInt("Controls", "p1Down", inputs[0][INPUT_DOWN].code);
	hge->Ini_SetInt("Controls", "p1Action", inputs[0][INPUT_ACTION].code);
	hge->Ini_SetInt("Controls", "p1Item", inputs[0][INPUT_ITEM].code);
	hge->Ini_SetInt("Controls", "p1PreviousItem", inputs[0][INPUT_LAST_ITEM].code);
	hge->Ini_SetInt("Controls", "p1NextItem", inputs[0][INPUT_NEXT_ITEM].code);

	hge->Ini_SetInt("Controls", "p1LeftK", inputs[0][INPUT_LEFT].whichDevice);
	hge->Ini_SetInt("Controls", "p1RightK", inputs[0][INPUT_RIGHT].whichDevice);
	hge->Ini_SetInt("Controls", "p1UpK", inputs[0][INPUT_UP].whichDevice);
	hge->Ini_SetInt("Controls", "p1DownK", inputs[0][INPUT_DOWN].whichDevice);
	hge->Ini_SetInt("Controls", "p1ActionK", inputs[0][INPUT_ACTION].whichDevice);
	hge->Ini_SetInt("Controls", "p1ItemK", inputs[0][INPUT_ITEM].whichDevice);
	hge->Ini_SetInt("Controls", "p1PreviousItemK", inputs[0][INPUT_LAST_ITEM].whichDevice);
	hge->Ini_SetInt("Controls", "p1NextItemK", inputs[0][INPUT_NEXT_ITEM].whichDevice);


	//Player 2
	hge->Ini_SetInt("Controls", "p2Left", inputs[1][INPUT_LEFT].code);
	hge->Ini_SetInt("Controls", "p2Right", inputs[1][INPUT_RIGHT].code);
	hge->Ini_SetInt("Controls", "p2Up", inputs[1][INPUT_UP].code);
	hge->Ini_SetInt("Controls", "p2Down", inputs[1][INPUT_DOWN].code);
	hge->Ini_SetInt("Controls", "p2Action", inputs[1][INPUT_ACTION].code);
	hge->Ini_SetInt("Controls", "p2Item", inputs[1][INPUT_ITEM].code);
	hge->Ini_SetInt("Controls", "p2PreviousItem", inputs[1][INPUT_LAST_ITEM].code);
	hge->Ini_SetInt("Controls", "p2NextItem", inputs[1][INPUT_NEXT_ITEM].code);


	hge->Ini_SetInt("Controls", "p2LeftK", inputs[1][INPUT_LEFT].whichDevice);
	hge->Ini_SetInt("Controls", "p2RightK", inputs[1][INPUT_RIGHT].whichDevice);
	hge->Ini_SetInt("Controls", "p2UpK", inputs[1][INPUT_UP].whichDevice);
	hge->Ini_SetInt("Controls", "p2DownK", inputs[1][INPUT_DOWN].whichDevice);
	hge->Ini_SetInt("Controls", "p2ActionK", inputs[1][INPUT_ACTION].whichDevice);
	hge->Ini_SetInt("Controls", "p2ItemK", inputs[1][INPUT_ITEM].whichDevice);
	hge->Ini_SetInt("Controls", "p2PreviousItemK", inputs[1][INPUT_LAST_ITEM].whichDevice);
	hge->Ini_SetInt("Controls", "p2NextItemK", inputs[1][INPUT_NEXT_ITEM].whichDevice);


	//Player 3
	hge->Ini_SetInt("Controls", "p3Left", inputs[2][INPUT_LEFT].code);
	hge->Ini_SetInt("Controls", "p3Right", inputs[2][INPUT_RIGHT].code);
	hge->Ini_SetInt("Controls", "p3Up", inputs[2][INPUT_UP].code);
	hge->Ini_SetInt("Controls", "p3Down", inputs[2][INPUT_DOWN].code);
	hge->Ini_SetInt("Controls", "p3Action", inputs[2][INPUT_ACTION].code);
	hge->Ini_SetInt("Controls", "p3Item", inputs[2][INPUT_ITEM].code);
	hge->Ini_SetInt("Controls", "p3PreviousItem", inputs[2][INPUT_LAST_ITEM].code);
	hge->Ini_SetInt("Controls", "p3NextItem", inputs[2][INPUT_NEXT_ITEM].code);

	hge->Ini_SetInt("Controls", "p3LeftK", inputs[2][INPUT_LEFT].whichDevice);
	hge->Ini_SetInt("Controls", "p3RightK", inputs[2][INPUT_RIGHT].whichDevice);
	hge->Ini_SetInt("Controls", "p3UpK", inputs[2][INPUT_UP].whichDevice);
	hge->Ini_SetInt("Controls", "p3DownK", inputs[2][INPUT_DOWN].whichDevice);
	hge->Ini_SetInt("Controls", "p3ActionK", inputs[2][INPUT_ACTION].whichDevice);
	hge->Ini_SetInt("Controls", "p3ItemK", inputs[2][INPUT_ITEM].whichDevice);
	hge->Ini_SetInt("Controls", "p3PreviousItemK", inputs[2][INPUT_LAST_ITEM].whichDevice);
	hge->Ini_SetInt("Controls", "p3NextItemK", inputs[2][INPUT_NEXT_ITEM].whichDevice);

}

//-----------------------------------------------------------------------------
// Name: loadInputs()
// Desc: Loads the input values from Smiley.ini
//-----------------------------------------------------------------------------
void Input::loadInputs() {

	//Player 1
	inputs[0][INPUT_LEFT].code = hge->Ini_GetInt("Controls", "p1Left", HGEK_LEFT);
	inputs[0][INPUT_RIGHT].code = hge->Ini_GetInt("Controls", "p1Right", HGEK_RIGHT);
	inputs[0][INPUT_UP].code = hge->Ini_GetInt("Controls", "p1Up", HGEK_UP);
	inputs[0][INPUT_DOWN].code = hge->Ini_GetInt("Controls", "p1Down", HGEK_DOWN);
	inputs[0][INPUT_ACTION].code = hge->Ini_GetInt("Controls", "p1Action", HGEK_CTRL);
	inputs[0][INPUT_ITEM].code = hge->Ini_GetInt("Controls", "p1Item", HGEK_C);
	inputs[0][INPUT_LAST_ITEM].code = hge->Ini_GetInt("Controls", "p1PreviousItem", HGEK_Z);
	inputs[0][INPUT_NEXT_ITEM].code = hge->Ini_GetInt("Controls", "p1NextItem", HGEK_X);

	inputs[0][INPUT_LEFT].whichDevice = hge->Ini_GetInt("Controls", "p1LeftK", 1);
	inputs[0][INPUT_RIGHT].whichDevice = hge->Ini_GetInt("Controls", "p1RightK", 1);
	inputs[0][INPUT_UP].whichDevice = hge->Ini_GetInt("Controls", "p1UpK", 1);
	inputs[0][INPUT_DOWN].whichDevice = hge->Ini_GetInt("Controls", "p1DownK", 1);
	inputs[0][INPUT_ACTION].whichDevice = hge->Ini_GetInt("Controls", "p1ActionK", 1);
	inputs[0][INPUT_ITEM].whichDevice = hge->Ini_GetInt("Controls", "p1ItemK", 1);
	inputs[0][INPUT_LAST_ITEM].whichDevice = hge->Ini_GetInt("Controls", "p1PreviousItemK", 1);
	inputs[0][INPUT_NEXT_ITEM].whichDevice = hge->Ini_GetInt("Controls", "p1NextItemK", 1);


	//Player 2
	inputs[1][INPUT_LEFT].code = hge->Ini_GetInt("Controls", "p2Left", HGEK_A);
	inputs[1][INPUT_RIGHT].code = hge->Ini_GetInt("Controls", "p2Right", HGEK_D);
	inputs[1][INPUT_UP].code = hge->Ini_GetInt("Controls", "p2Up", HGEK_W);
	inputs[1][INPUT_DOWN].code = hge->Ini_GetInt("Controls", "p2Down", HGEK_S);
	inputs[1][INPUT_ACTION].code = hge->Ini_GetInt("Controls", "p2Action", HGEK_SHIFT);
	inputs[1][INPUT_ITEM].code = hge->Ini_GetInt("Controls", "p2Item", HGEK_B);
	inputs[1][INPUT_LAST_ITEM].code = hge->Ini_GetInt("Controls", "p2PreviousItem", HGEK_N);
	inputs[1][INPUT_NEXT_ITEM].code = hge->Ini_GetInt("Controls", "p2NextItem", HGEK_M);

	inputs[1][INPUT_LEFT].whichDevice = hge->Ini_GetInt("Controls", "p2LeftK", 1);
	inputs[1][INPUT_RIGHT].whichDevice = hge->Ini_GetInt("Controls", "p2RightK", 1);
	inputs[1][INPUT_UP].whichDevice = hge->Ini_GetInt("Controls", "p2UpK", 1);
	inputs[1][INPUT_DOWN].whichDevice = hge->Ini_GetInt("Controls", "p2DownK", 1);
	inputs[1][INPUT_ACTION].whichDevice = hge->Ini_GetInt("Controls", "p2ActionK", 1);
	inputs[1][INPUT_ITEM].whichDevice = hge->Ini_GetInt("Controls", "p2ItemK", 1);
	inputs[1][INPUT_LAST_ITEM].whichDevice = hge->Ini_GetInt("Controls", "p2PreviousItemK", 1);
	inputs[1][INPUT_NEXT_ITEM].whichDevice = hge->Ini_GetInt("Controls", "p2NextItemK", 1);


	//Player 3
	inputs[2][INPUT_LEFT].code = hge->Ini_GetInt("Controls", "p3Left", HGEK_NUMPAD4);
	inputs[2][INPUT_RIGHT].code = hge->Ini_GetInt("Controls", "p3Right", HGEK_NUMPAD6);
	inputs[2][INPUT_UP].code = hge->Ini_GetInt("Controls", "p3Up", HGEK_NUMPAD8);
	inputs[2][INPUT_DOWN].code = hge->Ini_GetInt("Controls", "p3Down", HGEK_NUMPAD5);
	inputs[2][INPUT_ACTION].code = hge->Ini_GetInt("Controls", "p3Action", HGEK_NUMPAD1);
	inputs[2][INPUT_ITEM].code = hge->Ini_GetInt("Controls", "p3Item", HGEK_R);
	inputs[2][INPUT_LAST_ITEM].code = hge->Ini_GetInt("Controls", "p3PreviousItem", HGEK_T);
	inputs[2][INPUT_NEXT_ITEM].code = hge->Ini_GetInt("Controls", "p3NextItem", HGEK_Y);

	inputs[2][INPUT_LEFT].whichDevice = hge->Ini_GetInt("Controls", "p3LeftK", 1);
	inputs[2][INPUT_RIGHT].whichDevice = hge->Ini_GetInt("Controls", "p3RightK", 1);
	inputs[2][INPUT_UP].whichDevice = hge->Ini_GetInt("Controls", "p3UpK", 1);
	inputs[2][INPUT_DOWN].whichDevice = hge->Ini_GetInt("Controls", "p3DownK", 1);
	inputs[2][INPUT_ACTION].whichDevice = hge->Ini_GetInt("Controls", "p3ActionK", 1);
	inputs[2][INPUT_ITEM].whichDevice = hge->Ini_GetInt("Controls", "p3ItemK", 1);
	inputs[2][INPUT_LAST_ITEM].whichDevice = hge->Ini_GetInt("Controls", "p3PreviousItemK", 1);
	inputs[2][INPUT_NEXT_ITEM].whichDevice = hge->Ini_GetInt("Controls", "p3NextItemK", 1);

}

//-----------------------------------------------------------------------------
// Name: setEditMode()
// Desc: Sets the specified input's editMode to true and the rest to false
//-----------------------------------------------------------------------------
void Input::setEditMode(int whichPlayer, int whichInput) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < NUM_INPUTS; j++) {
			inputs[i][j].editMode = (i == whichPlayer && j == whichInput);
		}
	}
}

/**
 * Listens for the next input from the player (either keyboard or gamepad) and saves
 * that as the button for the specified control.
 */
void Input::listenForNewInput(int player, int control) {

	//Check keyboard buttons
	for (int i = 0; i < 255; i++) {
		if (hge->Input_KeyDown(i) && i != HGEK_LBUTTON && i != HGEK_ESCAPE) {
			//Save new control
			inputs[player][control].code = i;
			inputs[player][control].whichDevice = DEVICE_KEYBOARD;
			//Turn edit mode off
			setEditMode(-1, -1);
		}
	}

	//Check joystick
	if (joystickDirection ==  INPUT_LEFT) {
		inputs[player][control].code = JOYSTICK_LEFT;
		inputs[player][control].whichDevice = DEVICE_GAMEPAD1;
		setEditMode(-1, -1);
	} else if (joystickDirection ==  INPUT_RIGHT) {
		inputs[player][control].code = JOYSTICK_RIGHT;
		inputs[player][control].whichDevice = DEVICE_GAMEPAD1;
		setEditMode(-1, -1);
	} else if (joystickDirection ==  INPUT_DOWN) {
		inputs[player][control].code = JOYSTICK_DOWN;
		inputs[player][control].whichDevice = DEVICE_GAMEPAD1;
		setEditMode(-1, -1);
	} else if (joystickDirection ==  INPUT_UP) {
		inputs[player][control].code = JOYSTICK_UP;
		inputs[player][control].whichDevice = DEVICE_GAMEPAD1;
		setEditMode(-1, -1);
	}

	//Check gamepad buttons
	for (int i = 0; i < 225; i++) {
		if (gamePadButtons[i]) {
			//Save new control
			inputs[player][control].code = i;
			inputs[player][control].whichDevice = DEVICE_GAMEPAD1;
			//Turn edit mode off
			setEditMode(-1, -1);
		}
	}

}

/**
 * Sets the current joystick direction in a way that improves handling by taking
 * into consideration what the player probably wants to do when multiple directions
 * are pressed at once.
 */
void Input::updateJoystickDirection(DIJOYSTATE2 js) {
		
	//Update previous states
	for (int i = 0; i < 4; i++) {
		previousJoystickState[i] = joystickState[i];
	}

	//Update current states
	joystickState[INPUT_LEFT] = (js.lX < AXIS_MINCLICK);
	joystickState[INPUT_RIGHT] = (js.lX > AXIS_MAXCLICK);
	joystickState[INPUT_UP] = (js.lY < AXIS_MINCLICK);
	joystickState[INPUT_DOWN] = (js.lY > AXIS_MAXCLICK);

	//Determine if the joystick state has changed since last frame
	bool stateChange = false;
	for (int i = 0; i < 4; i++) {
		if (joystickState[i] != previousJoystickState[i]) stateChange = true;
	}

	if (stateChange) {

		//Give preference to the most recenty pressed direction so that
		//if the player is going one direction then presses an additonal
		//direction, the botonoid will move in the newest direction.
		//Example - frame 1 the player is going 100% right, then frame 2
		//		hes going 100% up right, the botonoid should move up.
		joystickDirection = -1;
		bool newDirection = false;
		for (int i = 0; i < 4; i++) {
			if (joystickState[i] && !previousJoystickState[i]) {
				joystickDirection = i;
				newDirection = true;
			}
		}

		//If no new direction was found this turn use the direction
		//returned by DirectInput in case the player released a direction.
		//Example - afer the previous example the player lets go of up and is
		//		only pressing right, the Botonoid should return to moving right.
		if (!newDirection) {
			for (int i = 0; i < 4; i++) {
				if (joystickState[i]) joystickDirection = i;
			}
		}

	}

}

/**
 * Returns a short description of a player's control
 */
std::string Input::getInputDescription(int player, int control) {
	
	std::string description;

	//Edit mode
	if (inputs[player][control].editMode) {
		description = "Press Button";
		return description;
	
	//Keyboard - use HGE function
	} else if (inputs[player][control].whichDevice == DEVICE_KEYBOARD) {
		description = hge->Input_GetKeyName(inputs[player][control].code);
		return description;
	
	//One of the gamepads - use terrible C string functions
	} else {

		//Gamepad number
		switch (inputs[player][control].whichDevice) {
			case DEVICE_GAMEPAD1:
				description = "GP1";
				break;
			case DEVICE_GAMEPAD2:
				description = "GP2";
				break;
			case DEVICE_GAMEPAD3:
				description = "GP3";
				break;
		}

		//Special case - direction pad
		switch (inputs[player][control].code) {
			case JOYSTICK_LEFT:
				description += " Left";
				return description;
			case JOYSTICK_RIGHT:
				description += " Right";
				return description;
			case JOYSTICK_UP:
				description += " Up";
				return description;
			case JOYSTICK_DOWN:
				description += " Down";
				return description;
		}

		//Button number
		description += " Button ";
		description += intToString(inputs[player][control].code);

		return description;

	}

}
