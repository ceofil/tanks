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

void Player::Update(Keyboard& kbd, const float dt, 
					RectF walls[], int indexWalls, 
					Player& other, 
					Ball balls[], int nBalls,
					const int up, const int down, const int left, const int right)
{
	
	bool rotationHappened = false;
	if (kbd.KeyIsPressed(left))
	{
		angle += rotationSpeed * dt;
		rotationHappened = true;
	}
	else if (kbd.KeyIsPressed(right))
	{
		angle -= rotationSpeed * dt;
		rotationHappened = true;
	}

	if (rotationHappened)
	{
		dir = AngleToVec2(angle);
	}
	
	if (kbd.KeyIsPressed(up))
	{
		pos += dir * speed * dt;
		DoPlayerCollision(other, dt);
		for (int i = 0; i <= indexWalls; i++)
		{
			DoWallCollision(walls[i], dir, dt);
		}
	}
	else if (kbd.KeyIsPressed(down))
	{
		pos -= dir * speed * dt;
		DoPlayerCollision(other, dt);
		for (int i = 0; i <= indexWalls; i++)
		{
			DoWallCollision(walls[i], dir*(-1.0f), dt);
		}
	}
	for (int i = 0; i <= nBalls; i++)
	{
		if (balls[i].IsSpawned())
		{
			if (IsOverLappingWith(balls[i].GetPosition(), Ball::radius))
			{
				balls[i].Destroy();
			}
		}
	}

}

RectF Player::GetRect() const
{
	return RectF(pos-Vec2(radius,radius),pos+Vec2(radius,radius));
}

void Player::DoWallCollision(const RectF & wall, const Vec2 dir, const float dt)
{
	const RectF rect = GetRect();
	if (rect.IsOverlappingWith(wall))
	{
		if (std::signbit(dir.x) == std::signbit((pos.x - wall.GetCenter().x)))
		{
			pos.y -= dir.y * speed * dt;
		}
		else
		{
			if (pos.x > wall.left && pos.x < wall.right) {
				pos.y -= dir.y * speed * dt;
			}
			else {
				if (pos.y > wall.top && pos.y < wall.bottom)
				{
					pos.x -= dir.x * speed * dt;
				}
				else
				{
					pos.x -= dir.x * speed * dt;
				}
				
			}
		}
	}
}

Vec2 Player::GetPos() const
{
	return pos;
}

Vec2 Player::GetDir() const
{
	return dir;
}

Vec2 Player::GetSpawnPoint() const
{
	return pos + dir * radius * 2.0f;
}

bool Player::IsOverLappingWith(const Vec2 other, float r)
{
	return (GetPos() - other).GetLength() <= radius + r;
}

void Player::DoPlayerCollision(Player & other, float dt)
{
	Vec2 otherPos = other.pos;
	while (IsOverLappingWith(otherPos, radius))
	{
		Move(  dir * -1.0f , dt);
	}
}

void Player::Move(Vec2 dir_in, float dt)
{
	pos += dir_in * speed * dt;
}

Vec2 Player::AngleToVec2(const float& angle)
{
	const float cos = std::cos(angle * pi / 180.0f);
	const float sin = std::sin(angle * pi / 180.0f);
	return Vec2(cos,-sin);  //because the framework y ascending from top to bottom
}
