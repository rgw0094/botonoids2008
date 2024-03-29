#ifndef _INPUT_H_
#define _INPUT_H_

#define STRICT
#include <windows.h>
#include <basetsd.h>
#include <dinput.h>
#include <commctrl.h>
#include <dinput.h>
#include "resource.h"
#include <string>

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

#define AXIS_MIN -1000			//when joystick is all the way left or up, it returns this
#define AXIS_MAX  1000			//when joystick is all the way right or down, it returns this
#define AXIS_MINCLICK -700		//when axis is below this value, it takes action for that direction
#define AXIS_MAXCLICK 700		//when axis is above this value, it takes action for that direction

//Codes for joystick
#define JOYSTICK_LEFT -5
#define JOYSTICK_UP -4
#define JOYSTICK_RIGHT -3
#define JOYSTICK_DOWN -2

//Inputs
#define NUM_INPUTS 8
#define INPUT_LEFT 0
#define INPUT_RIGHT 1
#define INPUT_UP 2
#define INPUT_DOWN 3
#define INPUT_ACTION 4				//Change color or build foundation
#define INPUT_ITEM 5				//Use item
#define INPUT_LAST_ITEM 6
#define INPUT_NEXT_ITEM 7

//Device types
#define DEVICE_KEYBOARD 1
#define DEVICE_GAMEPAD1 2
#define DEVICE_GAMEPAD2 3
#define DEVICE_GAMEPAD3 4

struct InputStruct {
	bool pressed, prevPressed, editMode;
	int whichDevice;
	int code;
	char name[30];
};

class Input {

public:
    
	//Functions
	Input();
	~Input();
	void InitInput(HWND hDlg);
	HRESULT InitDirectInput( HWND hDlg );
	VOID FreeDirectInput();
	void UpdateInput();
	bool buttonDown(int key, int player);
	bool buttonPressed(int key, int player);
	void toggleGamePad();
	void saveInputs();
	void loadInputs();
	void resetControls();
	void setEditMode(int player, int whichInput);
	void listenForNewInput(int player, int control);
	void updateJoystickDirection(DIJOYSTATE2 js);
	std::string getInputDescription(int player, int control);

	//Variables
	InputStruct inputs[3][NUM_INPUTS];
	bool joystickState[4];
	bool previousJoystickState[4];
	int joystickDirection;
	bool gamePadButtons[256];
	bool acquiredJoystick;
	bool deviceConnected;	//Whether or not there is an extra input device connected.

};



#endif