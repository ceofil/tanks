#include "Player.h"
#include <cmath>

Player::Player(const Vec2 in_pos, const Vec2 in_dir)
	:
	pos(in_pos),
	dir(in_dir)
{
}

void Player::Draw(Graphics & gfx)
{
	gfx.DrawCircle(pos, int(radius), Colors::White);

	Vec2 aim = pos + dir.GetNormalized() * radius;
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
		radian -= rotationSpeed * dt;
		rotationHappened = true;
	}
	else if (kbd.KeyIsPressed(VK_RIGHT))
	{
		radian += rotationSpeed * dt;
		rotationHappened = true;
	}
	if (rotationHappened)
	{
		const float x = std::cos(radian * pi / 180.0f);
		const float y = std::sin(radian * pi / 180.0f);
		dir = Vec2(x, y).GetNormalized();

		rotationHappened = false;
	}
	
}
