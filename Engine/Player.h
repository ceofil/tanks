#pragma once
#include "Graphics.h"
#include "Vec2.h"
#include "Keyboard.h"
class Player
{
public:
	Player(const Vec2 in_pos, const float in_angle);
	void Draw(Graphics& gfx) const;
	void Update(Keyboard& kbd, const float dt);
private:
	static constexpr float speed = 200.0f;
	static constexpr float rotationSpeed = 100.0f;
	static constexpr float radius = 20.0f;
	static constexpr float scopeRadius = 0.2f;
	static constexpr float pi = 3.14159265f;
	Vec2 AngleToVec2(const float& angle);

	Vec2 pos;
	Vec2 dir;
	float angle;
};