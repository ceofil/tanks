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
	gfx.DrawCircle(pos, radius, Color(200,200,200));

	Vec2 aim = pos + dir * radius;
	gfx.DrawCircle(aim, radius * scopeRadius, Colors::Red);
}

void Player::Update(Keyboard & kbd, const float dt, RectF& wall)
{
	
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
	
	if (kbd.KeyIsPressed(VK_UP))
	{
		pos += dir * speed * dt;
		DoWallCollision(wall, dir, dt);
	}
	else if (kbd.KeyIsPressed(VK_DOWN))
	{
		pos -= dir * speed * dt;
		DoWallCollision(wall, dir*(-1.0f), dt);
	}

}

RectF Player::GetRect() const
{
	return RectF(pos-Vec2(radius,radius),pos+Vec2(radius,radius));
}

void Player::DoWallCollision(const RectF & wall, const Vec2& dir, const float dt)
{
	const RectF rect = GetRect();
	if (rect.IsOverlappingWith(wall))
	{
		if (std::signbit(dir.x) == std::signbit((pos - wall.GetCenter()).x))
		{
			pos.y -= dir.y * speed * dt;
		}
		else
		{
			if (pos.x > wall.left && pos.x < wall.right) {
				pos.y -= dir.y * speed * dt;
			}
			else {
				pos.x -= dir.x * speed * dt;
			}
		}
	}
}

Vec2 Player::AngleToVec2(const float& angle)
{
	const float cos = std::cos(angle * pi / 180.0f);
	const float sin = std::sin(angle * pi / 180.0f);
	return Vec2(cos,-sin);  //because the framework y ascending from top to bottom
}
