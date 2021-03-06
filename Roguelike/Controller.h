#pragma once
#include "FileManager.h"
#include "World.h"

#include <SDL/SDL_events.h>
#include <Engine/Camera.h>
#include <Engine/UIButton.h>
#include <Engine/UITextbox.h>
#include <Engine/Texture.h>

class PropertySet;

namespace PlayerEnums {
	enum EditMode { NONE, PLACE, DELETE };

	enum Direction { RIGHT, UP, LEFT, DOWN };
}

using namespace GUI;

class Controller
{
private:
	UIContainer _menuBar; //Parents MUST be declared before children or they will not be able to destruct properly
	UIButton _loadButton, _saveButton;
	UITextBox _namebox;

	UIContainer _propertyBar;
	UIRect _propertyBarBG;

	UIText _counter;
	//////////////////////////////////
	static bool _inputLock;
	static const char* levelname;

	struct {
		signed char layer;
		unsigned char id;
		int x;
		int y;
	} _selection;

	Texture _tiletexture, _symboltexture;

	bool _usingUI;

	int _mouseX, _mouseY;
	float _moveX, _moveY, _CameraScale;

	PlayerEnums::EditMode _editMode;
	bool _entMode;
	//PlayerEnums::PlacementEmun _placeMode;

	bool MovementInputs[4];

	void setMovement(PlayerEnums::Direction, bool);

	static void save(), load(), setInputState(bool);
public:
	float speed;

	Controller();
	~Controller() {};

	void init();
	void render(float deltaTime, Camera&);
	void input(SDL_Event event, int);

	void loadProperties(const PropertySet&);
};
