#include "Controller.h"

#include <Engine/LineRenderer.h>
#include <Engine/SpriteRenderer.h>
#include <Engine/ResourceManager.h>
#include <iostream>

Level* Controller::currentLevel;
bool Controller::_inputLock;
const char* Controller::levelname = "sample.lvl";

using namespace PlayerEnums;

Controller::Controller() : 
	speed(768), 
	_menuBar(0, 32, 1, 32, NORMALISED_WIDTH | ONEMINUS_Y),
	_namebox(0, 0, 1/3.0f, 1, NORMALISED_WIDTH | NORMALISED_HEIGHT), 
	_loadButton(1/3.0f, 0, 1/3.0f, 1, NORMALISED_X | NORMALISED_WIDTH | NORMALISED_HEIGHT),
	_saveButton(2/3.0f, 0, 1/3.0f, 1, NORMALISED_X | NORMALISED_WIDTH | NORMALISED_HEIGHT),
	_counter(0, 0, 1, 32, NORMALISED_WIDTH)
{}

void Controller::save() {
	if (levelname != nullptr)
		currentLevel->save(levelname);
}

void Controller::load() {
	if (levelname != nullptr)
		currentLevel->load(levelname);
}

void Controller::setInputState(bool typing) {
	if (typing) SDL_StartTextInput();
	else SDL_StopTextInput();

	_inputLock = typing;
}

void Controller::init(Font& UIFont) {
	_namebox.label = "Sample.lvl";
	_namebox.setColour(NormalisedColour(0,0,1));
	_namebox.setSelectColour(NormalisedColour(0,0,0.5));
	_namebox.label.setColour(NormalisedColour(1,1,1,1));
	_namebox.onStateChanged = setInputState;

	_loadButton.label = "LOAD";
	_loadButton.label.setColour(NormalisedColour(0,0,0,1));
	_loadButton.setColour(NormalisedColour(1, 1, 0));
	_loadButton.setHoverColour(NormalisedColour(1, 0, 0));
	_loadButton.onClick = load;

	_saveButton.label = "SAVE";
	_saveButton.label.setColour(NormalisedColour(0, 0, 0, 1));
	_saveButton.setColour(NormalisedColour(1, 1, 0));
	_saveButton.setHoverColour(NormalisedColour(1, 0, 0));
	_saveButton.onClick = save;

	_counter = "TileID : 0|Layer : 0";
	_counter.setColour(NormalisedColour(1,1,1,1));

	_namebox.label.setFont(UIFont);
	_saveButton.label.setFont(UIFont);
	_loadButton.label.setFont(UIFont);
	_counter.setFont(UIFont);

	GlobalUI::add(_counter);
	GlobalUI::add(_menuBar);
	_menuBar.addElement(_namebox);
	_menuBar.addElement(_loadButton);
	_menuBar.addElement(_saveButton);
	/////////////////////////////////
	_tiletexture = ResourceManager::getTexture("Game/Textures/tiles.png");
	_symboltexture = ResourceManager::getTexture("Game/Textures/symbols.png");

	_level.tileSheet = &_tiletexture;
	_level.editorTileSheet = &_symboltexture;

	currentLevel = &_level;
}

int gridSnap(int i, int snap) {
	if (i % snap > 0)
		return i - (i % snap);
	else return i - snap - (i % snap);
}

void Controller::render(float deltaTime,Camera2D& cam) {
	cam.move(_moveX * speed * deltaTime, _moveY * speed * deltaTime);

	glm::vec2 f = cam.screentoWorld(_mouseX,_mouseY);
	_currentTile.x = gridSnap((int)f.x,64);
	_currentTile.y = gridSnap((int)f.y,64);

	switch (_editMode) {
		case PLACING:
			if (_placeMode == TILE)
				_level.edit(_currentTile);
			else
				_level.setSpawnPoint(_currentTile.x,_currentTile.y);
			break;
		case DELETING:
			_level.edit(_currentTile,true);
			break;
	}
	
	SpriteRenderer::UseProgram(cam);
		for (int layer = -16;layer < _currentTile.layer;layer++)
			_level.drawSprites(cam,layer);
	SpriteRenderer::UnuseProgram();

	LineRenderer::render(cam);

	SpriteRenderer::UseProgram(cam);
	if (_editMode != DELETING && !_usingUI)
		if (_placeMode == TILE)
			SpriteRenderer::drawSprite(_tiletexture, (float)_currentTile.x, (float)_currentTile.y, 64.0f, 64.0f, Colour(255, 255, 255, 128), 0.0f, 8, _currentTile.TileID);
		else
			SpriteRenderer::drawSprite(_symboltexture, (float)_currentTile.x, (float)_currentTile.y, 64.0f, 64.0f, Colour(255, 255, 255, 128), 0.0f, 8, _currentTile.TileID);

	_level.drawEditorSprites();

		for (int layer = _currentTile.layer; layer < 16; layer++)
			_level.drawSprites(cam,layer);
	SpriteRenderer::UnuseProgram();
}

void Controller::setMovement(Direction dir,bool s) {
	MovementInputs[dir] = s;

	_moveX = (float)(MovementInputs[LEFT] ? (MovementInputs[RIGHT] ? 0 : -1) : MovementInputs[RIGHT] ? 1 : 0);
	_moveY = (float)(MovementInputs[UP] ? (MovementInputs[DOWN] ? 0 : 1) : MovementInputs[DOWN] ? -1 : 0);
}

void Controller::input(SDL_Event event, int screenh)
{
	SDL_GetMouseState(&_mouseX,&_mouseY);
	_mouseY = screenh - _mouseY;

	_usingUI = GlobalUI::updateMousePosition(_mouseX, _mouseY);

	if (event.type == SDL_TEXTINPUT) {
		_namebox.textInput(event.text.text[0]);
		levelname = _namebox.label.text.c_str();
	}

	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (!_usingUI)
			switch (event.button.button) {
			case SDL_BUTTON_LEFT:
				_editMode = PLACING; break;
			case SDL_BUTTON_RIGHT:
				_editMode = DELETING; break;
			}
		GlobalUI::click();
	}
	else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_BACKSPACE && _namebox.label.text.length() > 0) {
		_namebox.label.text.pop_back();
	}

	if (_inputLock)return;

	if (event.type == SDL_KEYDOWN) {
		std::string path;
		switch (event.key.keysym.sym) {
		case SDLK_w:setMovement(UP,true); break;
		case SDLK_s:setMovement(DOWN, true); break;
		case SDLK_a:setMovement(LEFT, true); break;
		case SDLK_d:setMovement(RIGHT, true); break;

		//case SDLK_SPACE:_placeMode = !_placeMode; break;
		case SDLK_r:_currentTile.TileID--; break;
		case SDLK_t:_currentTile.TileID++; break;
		case SDLK_LEFTBRACKET:_currentTile.layer--; break;
		case SDLK_RIGHTBRACKET:_currentTile.layer++; break;
		}

		_counter = "TileID : " + std::to_string(_currentTile.TileID) + "|Layer : " + std::to_string(_currentTile.layer);
	}
	else if (event.type == SDL_KEYUP) {
		switch (event.key.keysym.sym) {
		case SDLK_w:setMovement(UP, false); break;
		case SDLK_s:setMovement(DOWN, false); break;
		case SDLK_a:setMovement(LEFT, false); break;
		case SDLK_d:setMovement(RIGHT, false); break;
		}
	}
	else if (event.type == SDL_MOUSEWHEEL) {
		if (event.wheel.y > 0)
			_CameraScale = 0.1f;
		else if (event.wheel.y < 0)
			_CameraScale = -0.1f;
	}
	else if (event.type == SDL_MOUSEBUTTONUP) {
		_editMode = NONE;
	}
}