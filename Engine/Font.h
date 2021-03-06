#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <map>

class String;
class Shader;
typedef unsigned int GLuint;

struct Character {
	GLuint TextureID;
	glm::ivec2 Size;		// Size of glyph
	glm::ivec2 Bearing;		// Offset from baseline to left/top of glyph
	GLuint Advance;			// Horizontal offset to advance to next glyph
};

class Font
{
private:
	FT_Face _face;
	std::map <char, Character> _chars;
	GLuint _vao, _vbo;
	int _pointsize;
public:
	Font();
	~Font();

	bool loaded = false;

	int getPointSize() { return _pointsize; };
	void init(FT_Library& lib,const char* path,int);	//Creates an FT Face, creates the character map, and configures texture properties
	void drawString(const String& text, float x, float y, float scale);	//Renders a string to the screen. Call from within a shader program!
	void drawString(const String& text, float x, float y, glm::vec4 Colour, Shader& s) { drawString(text, x, y, _pointsize, Colour, s); }; //Renders a string to the screen. Call from within a shader program!
	void drawString(const String& text, float x, float y, int pointsize, glm::vec4 Colour, Shader& s); //Renders a string to the screen. Call from within a shader program!
};
