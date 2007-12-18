#ifndef _INPUT_H_
#define _INPUT_H_

#define STRICT
#include <windows.h>
#include <basetsd.h>
#include <dinput.h>
#include "resource.h"

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

#define AXIS_MIN -1000			//when joystick is all the way left or up, it returns this
#define AXIS_MAX  1000			//when joystick is all the way right or down, it returns this
#define AXIS_MINCLICK -700		//when axis is below this value, it takes action for that direction
#define AXIS_MAXCLICK 700		//when axis is above this value, it takes action for that direction

//Inputs
#define NUM_INPUTS 8
#define INPUT_LEFT 0
#define INPUT_RIGHT 1
#define INPUT_UP 2
#define INPUT_DOWN 3
#define INPUT_ATTACK 4
#define INPUT_ABILITY 5
#define INPUT_PREVIOUS_ABILITY 6
#define INPUT_NEXT_ABILITY 7

struct InputStruct {
	bool pressed, prevPressed, editMode;
	int keyCode, padCode;
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
	bool keyDown(int key);
	bool keyPressed(int key);
	void toggleGamePad();
	void saveInputs();
	void loadInputs();
	void setEditMode(int whichInput);

	//Variables
	InputStruct inputs[NUM_INPUTS];
	bool gamePadButtonPressed[128];
	bool useGamePad;
	bool acquiredJoystick;

};



#endif