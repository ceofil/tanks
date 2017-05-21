#include "Player.h"
#include <cmath>

Player::Player(const Vec2 in_pos, const float in_angle, Color c)
	:
	startPos(in_pos),
	angle (in_angle),
	c(c)
{
	startDir =  AngleToVec2(angle);
	NewRound();
}

void Player::Draw(Graphics & gfx) const
{
	gfx.DrawCircle(pos, radius, c);

	Vec2 aim = pos + dir * radius;
	gfx.DrawCircle(aim, radius * scopeRadius, Colors::White);
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
				if (HP > 1)
				{
					LowerHP(1);
				}
				else
				{
					NewRound();
					other.NewRound();
					other.AddToScore();
					for (int j = 0; j <= nBalls; j++)
					{
						if (balls[j].IsSpawned())
						{
							balls[j].Destroy();
						}
					}
				}
			}
		}
	}

	KeepInsideScreen(RectF(Vec2(0.0f, 0.0f), Vec2(float(Graphics::ScreenWidth), float(Graphics::ScreenHeight - 35))));
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
					if (std::abs(dir.y) > std::abs(dir.x))
					{
						pos.y -= dir.y * speed * dt;
					}
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
	return pos + dir * (radius+Ball::radius);
}

Color Player::GetColor() const
{
	return c;
}

int Player::GetScore() const
{
	return score;
}

int Player::GetHP() const
{
	return HP;
}

void Player::AddToScore()
{
	score++;
}

void Player::LowerHP(int dmg)
{
	HP -= dmg;
}

void Player::NewRound()
{
	pos = startPos;
	dir = startDir;
	HP = maxHP;
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

void Player::KeepInsideScreen(const RectF & walls)
{
	const RectF rect = GetRect();
	if (rect.left < walls.left)
	{
		pos.x += walls.left - rect.left;
	}
	else if (rect.right > walls.right)
	{
		pos.x -= rect.right - walls.right;
	}
	if (rect.top < walls.top)
	{
		pos.y += walls.top - rect.top;
	}
	else if (rect.bottom > walls.bottom)
	{
		pos.y -= rect.bottom - walls.bottom;
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
