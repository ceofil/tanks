#include "Player.h"
#include <cmath>

Player::Player(const Vec2 in_pos, const float in_angle)
	:
	pos(in_pos),
	angle (in_angle)
{
	dir = AngleToVec2(angle);
}

void Player::Draw(Graphics & gfx) const
{
	gfx.DrawCircle(pos, int(radius), Colors::White);

	Vec2 aim = pos + dir * radius;
	gfx.DrawCircle(aim, int(radius) / 5, Colors::Red);
}

void Player::Update(Keyboard & kbd, const float dt)
{
	if (kbd.KeyIsPressed(VK_UP))
	{
		pos += dir.GetNormalized() * speed * dt;
	}
	else if (kbd.KeyIsPressed(VK_DOWN))
	{
		pos -= dir.GetNormalized() * speed * dt;
	}

	bool rotationHappened = false;
	if (kbd.KeyIsPressed(VK_LEFT))
	{
		angle += rotationSpeed * dt;
		rotationHappened = true;
	}
	else if (kbd.KeyIsPressed(VK_RIGHT))
	{
		angle -= rotationSpeed * dt;
		rotationHappened = true;
	}

	if (rotationHappened)
	{
		dir = AngleToVec2(angle);
	}
	
}

Vec2 Player::AngleToVec2(const float& angle)
{
	const float cos = std::cos(angle * pi / 180.0f);
	const float sin = std::sin(angle * pi / 180.0f);
	return Vec2(cos,-sin);  //because the framework y ascending from top to bottom
}
