#include "button.h"
#include "game.h"

extern HGE *hge;
extern hgeResourceManager *resources;

Button::Button(float _x, float _y, char* _text) {
	width = 248.0f;
	height = 71.0f;
	x = _x;
	y = _y;
	strcpy(text, _text);
	collisionBox = new hgeRect(x, y, x+width, y+height);
	highlighted = false;
}

Button::~Button() {
	delete collisionBox;
}

void Button::draw(float dt) {
	
	//Draw graphic
	if (highlighted) {
		resources->GetSprite("highlightedButtonGraphic")->Render(x,y);
	} else {
		resources->GetSprite("buttonGraphic")->Render(x,y);
	}

	//Draw text
	resources->GetFont("button")->printf(x + width/2.0f, y + 20.0f, HGETEXT_CENTER, text);

}

void Button::update(float mouseX, float mouseY) {
	highlighted = collisionBox->TestPoint(mouseX, mouseY);
}

bool Button::isClicked() {
	return (highlighted && hge->Input_KeyDown(HGEK_LBUTTON));
}