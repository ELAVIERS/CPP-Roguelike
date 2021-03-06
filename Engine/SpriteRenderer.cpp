#include "SpriteRenderer.h"

#include "Camera.h"
#include "Texture.h"
#include "Utility.h"
#include <GL/glew.h>
#include <GLM/gtc/matrix_transform.hpp>

GLuint SpriteRenderer::_vbo, SpriteRenderer::_vao;
int SpriteRenderer::_currentIndex, SpriteRenderer::_divisions;
Colour SpriteRenderer::_colour;
glm::vec2 SpriteRenderer::_UVOffset;
Shader SpriteRenderer::_shader;

using namespace glm;

void SpriteRenderer::drawSprite(Shader& shader, const Texture &t, float x, float y, float width, float height, float angle, int index) {
	if (index != _currentIndex)setIndex(index);

	mat4 transform;
	transform = translate(transform, vec3(width / 2 + x, height / 2 + y, 0));
	if (angle != 0)transform = rotate(transform, angle, vec3(0, 0, 1));
	transform = translate(transform, vec3(-width / 2, -height / 2, 0));
	transform = scale(transform, vec3(width, height, 1));

	shader.setMat4("transform", transform);
	shader.set2f("UVOffset", _UVOffset.x, _UVOffset.y);

	glActiveTexture(GL_TEXTURE_2D);
	t.bind();
	glBindVertexArray(_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glActiveTexture(0);
}

void SpriteRenderer::setIndex(int index) {
	_currentIndex = index;
	_UVOffset = Utility::getOffsetOfUVIndex(index,_divisions);
}

void SpriteRenderer::setUVData(unsigned char divisions, const Colour& colour) {
	_divisions = divisions;
	setIndex(_currentIndex);
	Vertex poo[6];

	float uv = 1.0f / divisions;
	poo[0].setPosition(0, 1); poo[0].setUv(0, uv);	poo[0].colour = colour;
	poo[1].setPosition(1, 0); poo[1].setUv(uv, 0);	poo[1].colour = colour;
	poo[2].setPosition(0, 0); poo[2].setUv(0, 0);	poo[2].colour = colour;
	poo[3].setPosition(0, 1); poo[3].setUv(0, uv);	poo[3].colour = colour;
	poo[4].setPosition(1, 1); poo[4].setUv(uv, uv); poo[4].colour = colour;
	poo[5].setPosition(1, 0); poo[5].setUv(uv, 0);	poo[5].colour = colour;

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER,_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(poo), poo);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray(0);
}

void SpriteRenderer::init() 
{
	_currentIndex = _divisions = -1;

	glGenVertexArrays(1,&_vao);
	glGenBuffers(1, &_vbo);

	glBindVertexArray(_vao);//bind Vertex Array Object
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);//bind Vertex Buffer Object
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 6, nullptr, GL_STATIC_DRAW);		//Update buffer data
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));		//0-position
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, colour));	//1-colour
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, uv));				//2-uvs
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);//unbind Vertex Buffer Object
	glBindVertexArray(0);//unbind Vertex Array Object

	_shader.loadPreset(ShaderPreset::TRANSFORM_SPRITE);
	_shader.link();

	std::printf("SpriteRenderer initialised (VAO:%d,VBO:%d)\n",_vao,_vbo);
}
