#include "Input.h"

#include <windows.h>
#include <basetsd.h>
#include <dinput.h>
#include "resource.h"
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
	useGamePad = false;
	
	//Initialize input states
	for (int i = 0; i < NUM_INPUTS; i++) {
		inputs[i].pressed = inputs[i].prevPressed = false;
	}

	//Load controls
	loadInputs();

	//Set control names
	strcpy(inputs[INPUT_LEFT].name, "Move Left");
	strcpy(inputs[INPUT_RIGHT].name, "Move Right");
	strcpy(inputs[INPUT_DOWN].name, "Move Down");
	strcpy(inputs[INPUT_UP].name, "Move Up");
	strcpy(inputs[INPUT_ATTACK].name, "Attack/Select");
	strcpy(inputs[INPUT_ABILITY].name, "Use Ability");
	strcpy(inputs[INPUT_NEXT_ABILITY].name, "Next Ability");
	strcpy(inputs[INPUT_PREVIOUS_ABILITY].name, "Previous Ability");

	//Set edit modes to false
	for (int i = 0; i < NUM_INPUTS; i++) {
		inputs[i].editMode = false;	
	}

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
	for (int i = 0; i < NUM_INPUTS; i++) {
		inputs[i].prevPressed = inputs[i].pressed;
	}

	//Update Game Pad
	if (useGamePad) {

		HRESULT     hr;
		CHAR        strText[128]; // Device state text
		DIJOYSTATE2 js;           // DInput joystick state 
		CHAR*       str;

	    if(NULL == g_pJoystick) return;

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
				
		//Joystick input
		inputs[INPUT_LEFT].pressed = (js.lX < AXIS_MINCLICK);
		inputs[INPUT_RIGHT].pressed = (js.lX > AXIS_MAXCLICK);
		inputs[INPUT_UP].pressed = (js.lY < AXIS_MINCLICK);
		inputs[INPUT_DOWN].pressed = (js.lY > AXIS_MAXCLICK);

		//Update buttons
		for (int i = 0; i < 128; i++) {
			gamePadButtonPressed[i] = js.rgbButtons[i];
		}

		//Gamepad button input
		for (int i = INPUT_ATTACK; i <= INPUT_NEXT_ABILITY; i++) {
			inputs[i].pressed = gamePadButtonPressed[inputs[i].padCode];
		}

	//Update Keyboard 
	} else {
		for (int i = 0; i < NUM_INPUTS; i++) {
			inputs[i].pressed = hge->Input_GetKeyState(inputs[i].keyCode);
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
		toggleGamePad();
        MessageBox( NULL, "Input Device not found.", 
                    "Smiley's Maze Hunt 3", 
                    MB_ICONERROR | MB_OK );
        EndDialog( hDlg, 0 );
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
	if( FAILED( g_pJoystick->SetProperty( DIPROP_RANGE, &diprg.diph ) ) )
		return DIENUM_STOP;

    // Set the UI to reflect what axes the joystick supports
	if (pdidoi->guidType == GUID_XAxis) {
        EnableWindow( GetDlgItem( hDlg, IDC_X_AXIS ), TRUE );
        EnableWindow( GetDlgItem( hDlg, IDC_X_AXIS_TEXT ), TRUE );
	}
	if (pdidoi->guidType == GUID_YAxis) {
        EnableWindow( GetDlgItem( hDlg, IDC_Y_AXIS ), TRUE );
        EnableWindow( GetDlgItem( hDlg, IDC_Y_AXIS_TEXT ), TRUE );
	}
	if (pdidoi->guidType == GUID_ZAxis) {
        EnableWindow( GetDlgItem( hDlg, IDC_Z_AXIS ), TRUE );
        EnableWindow( GetDlgItem( hDlg, IDC_Z_AXIS_TEXT ), TRUE );
	}
	if (pdidoi->guidType == GUID_RxAxis) {
        EnableWindow( GetDlgItem( hDlg, IDC_X_ROT ), TRUE );
        EnableWindow( GetDlgItem( hDlg, IDC_X_ROT_TEXT ), TRUE );
	}
	if (pdidoi->guidType == GUID_RyAxis) {
        EnableWindow( GetDlgItem( hDlg, IDC_Y_ROT ), TRUE );
        EnableWindow( GetDlgItem( hDlg, IDC_Y_ROT_TEXT ), TRUE );
	}
	if (pdidoi->guidType == GUID_RzAxis) {
        EnableWindow( GetDlgItem( hDlg, IDC_Z_ROT ), TRUE );
        EnableWindow( GetDlgItem( hDlg, IDC_Z_ROT_TEXT ), TRUE );
	}
	if (pdidoi->guidType == GUID_Slider) {
        EnableWindow( GetDlgItem( hDlg, IDC_SLIDER0 ), TRUE );
        EnableWindow( GetDlgItem( hDlg, IDC_SLIDER0_TEXT ), TRUE );
        EnableWindow( GetDlgItem( hDlg, IDC_SLIDER1 ), TRUE );
        EnableWindow( GetDlgItem( hDlg, IDC_SLIDER1_TEXT ), TRUE );
    }

    return DIENUM_CONTINUE;
}

//-----------------------------------------------------------------------------
// Name: keyDown()
// Desc: Returns whether or not the specified key is currently pressed.
//-----------------------------------------------------------------------------
bool Input::keyDown(int key) {
	return inputs[key].pressed;
}

//-----------------------------------------------------------------------------
// Name: keyPressed()
// Desc: Returns whether or not the specified key was pressed this frame.
//-----------------------------------------------------------------------------
bool Input::keyPressed(int key) {
	return (inputs[key].pressed && !inputs[key].prevPressed);
}

//-----------------------------------------------------------------------------
// Name: toggleGamePad()
// Desc: Toggles between game pad and keyboard.
//-----------------------------------------------------------------------------
void Input::toggleGamePad() {
	useGamePad = !useGamePad;
	//Initialize gamepad
	if (useGamePad) {
		InitInput(hge->System_GetState(HGE_HWND));
	//Release gamepad
	} else {
		FreeDirectInput();
	}
	UpdateInput();
}

//-----------------------------------------------------------------------------
// Name: saveInputs()
// Desc: Saves the input values to Smiley.ini
//-----------------------------------------------------------------------------
void Input::saveInputs() {
	//Load keyboard controls
	hge->Ini_SetInt("Controls","kbLeft",inputs[INPUT_LEFT].keyCode);
	hge->Ini_SetInt("Controls","kbRight",inputs[INPUT_RIGHT].keyCode);
	hge->Ini_SetInt("Controls","kbUp",inputs[INPUT_UP].keyCode);
	hge->Ini_SetInt("Controls","kbDown",inputs[INPUT_DOWN].keyCode);
	hge->Ini_SetInt("Controls","kbAttack",inputs[INPUT_ATTACK].keyCode);
	hge->Ini_SetInt("Controls","kbAbility",inputs[INPUT_ABILITY].keyCode);
	hge->Ini_SetInt("Controls","kbNext",inputs[INPUT_NEXT_ABILITY].keyCode);
	hge->Ini_SetInt("Controls","kbPrevious",inputs[INPUT_PREVIOUS_ABILITY].keyCode);

	//Load gamepad controls
	hge->Ini_SetInt("Controls","gpAttack",inputs[INPUT_ATTACK].padCode);
	hge->Ini_SetInt("Controls","gpAbility",inputs[INPUT_ABILITY].padCode);
	hge->Ini_SetInt("Controls","gpNext",inputs[INPUT_NEXT_ABILITY].padCode);
	hge->Ini_SetInt("Controls","gpPrevious",inputs[INPUT_PREVIOUS_ABILITY].padCode);
}

//-----------------------------------------------------------------------------
// Name: loadInputs()
// Desc: Loads the input values from Smiley.ini
//-----------------------------------------------------------------------------
void Input::loadInputs() {
	//Load keyboard controls
	inputs[INPUT_LEFT].keyCode = hge->Ini_GetInt("Controls","kbLeft",HGEK_LEFT);
	inputs[INPUT_RIGHT].keyCode = hge->Ini_GetInt("Controls","kbRight",HGEK_RIGHT);
	inputs[INPUT_UP].keyCode = hge->Ini_GetInt("Controls","kbUp",HGEK_UP);
	inputs[INPUT_DOWN].keyCode = hge->Ini_GetInt("Controls","kbDown",HGEK_DOWN);
	inputs[INPUT_ATTACK].keyCode = hge->Ini_GetInt("Controls","kbAttack",HGEK_SHIFT);
	inputs[INPUT_ABILITY].keyCode = hge->Ini_GetInt("Controls","kbAbility",HGEK_CTRL);
	inputs[INPUT_NEXT_ABILITY].keyCode = hge->Ini_GetInt("Controls","kbNext",HGEK_X);
	inputs[INPUT_PREVIOUS_ABILITY].keyCode = hge->Ini_GetInt("Controls","kbPrevious",HGEK_Z);

	//Load gamepad controls
	inputs[INPUT_ATTACK].padCode = hge->Ini_GetInt("Controls","gpAttack",3);
	inputs[INPUT_ABILITY].padCode = hge->Ini_GetInt("Controls","gpAbility",2);
	inputs[INPUT_NEXT_ABILITY].padCode = hge->Ini_GetInt("Controls","gpNext",4);
	inputs[INPUT_PREVIOUS_ABILITY].padCode = hge->Ini_GetInt("Controls","gpPrevious",5);
}

//-----------------------------------------------------------------------------
// Name: setEditMode()
// Desc: Sets the specified input's editMode to true and the rest to false
//-----------------------------------------------------------------------------
void Input::setEditMode(int whichInput) {
	for (int i = 0; i < NUM_INPUTS; i++) {
		inputs[i].editMode = (i == whichInput);
	}
}