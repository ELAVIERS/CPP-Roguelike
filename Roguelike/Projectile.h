#pragma once

#include <Engine/Sprite.h>
#include <Engine/Camera2D.h>

class Projectile
{
private:
	Sprite _sprite;
public:
	float speed;

	Projectile();

	void init(int, int, int, std::string);
	void setDirection(float Angle);
	void render(float DeltaTime);
};