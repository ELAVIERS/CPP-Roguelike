#include "Editor.h"

#include <Engine/LineRenderer.h>
#include <Engine/SpriteRenderer.h>
#include <SDL/SDL.h>
#include <GL/glew.h>

#include <Engine/GUI.h>

#include "FileManager.h"

void Editor::start() {
	//FileManager::writeLevelFile(std::vector<Tile> {Tile{ 0,1,2 }, Tile{3,4,5}},"poo.poo");//Level loading test

	SDL_Init(SDL_INIT_EVERYTHING);
	_window.create("Level Editor", screenX, screenY, SDL_WINDOW_RESIZABLE);

	SDL_GL_SetSwapInterval(0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	glClearColor(.125, .125, .125, 1);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	////////////////////////////////////////////////////////////////////////

	LineRenderer::init();
	SpriteRenderer::init();

	_fontShader.loadPreset(ShaderPreset::FONT);
	_fontShader.link();

	FT_Library ft;
	FT_Init_FreeType(&ft);
	_font.init(ft, "Game/Fonts/font.ttf", 64);
	///////////////////////////////////////////////////////////////////////
	
	_camera.SetViewportSize(screenX, screenY);

	GlobalUI::setCameraSize(screenX, screenY);

	_controller.init(_font);

	////////////////////////////////////////////////////////////////////////
	Colour c;
	float unitSize = 64.0f;
	int lineCount = 256;

	for (int x = -lineCount; x <= lineCount; x++) {
		c = (x == 0) ? Colour(32,128,255) : Colour(0,0,0);
		LineRenderer::drawLine(x * unitSize, -lineCount * unitSize, x * unitSize, lineCount * unitSize, c);
		LineRenderer::drawLine(-lineCount * unitSize, x * unitSize, lineCount * unitSize, x * unitSize, c);
	}

	running = true;
	while (running) {

		_frameCounter.begin();
		windowInput();
		render(_frameCounter.deltaTime);
		_frameCounter.end();

	}

}

void Editor::render(float deltaTime) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearDepth(1);

	LineRenderer::render(_camera);

	_controller.render(deltaTime,_camera);

	GlobalUI::render(_fontShader);

	_fontShader.useProgram();
	_fontShader.setMat4("projection",_camera.getScreenMatrix());
	GlobalUI::render(_fontShader);
	_fontShader.unUseProgram();

	_window.swapBuffer();

}

void Editor::windowInput() {
	static SDL_Event event;
	if (SDL_PollEvent(&event) == 1) {
		_controller.input(event,screenY);
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
			running = false;
		if (event.type == SDL_QUIT)
			running = false;
		if (event.type == SDL_WINDOWEVENT)
			if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
				screenX = event.window.data1;
				screenY = event.window.data2;
				glViewport(0,0,screenX,screenY);
				_camera.SetViewportSize(screenX,screenY);
				GlobalUI::setCameraSize(screenX,screenY);
			}
	}
}