#pragma once
#include "Graphics.h"
#include "Vec2.h"
#include "Keyboard.h"
class Player
{
public:
	Player(const Vec2 in_pos, const Vec2 in_dir);
	void Draw(Graphics& gfx);
	void Update(Keyboard& kbd, const float dt);
private:
	static constexpr float speed = 200.0f;
	static constexpr float rotationSpeed = 100.0f;
	static constexpr float radius = 20.0f;
	static constexpr float scopeRadius = 10.0f;

	static constexpr float pi = 3.14159265f;
	Vec2 pos;
	Vec2 dir;
	float radian = 0.0f;
};