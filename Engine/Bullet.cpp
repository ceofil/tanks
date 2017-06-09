#include "Bullet.h"

Bullet::Bullet( const Vec2 & pos_in,const Vec2 & dir_in )
	:
	pos( pos_in )
{
	SetDirection( dir_in );
}

void Bullet::Draw( Graphics & gfx ) const
{
	gfx.DrawCircle(pos, radius, Colors::White);
}

void Bullet::Update( float dt, RectF walls[], int indexWalls, Sound& wallBounceSound)
{
	pos += vel * dt;
	lifeTime -= dt;
	DoWallCollision(RectF(Vec2(0.0f, 0.0f), Vec2(float(Graphics::ScreenWidth), float(Graphics::ScreenHeight-35))), wallBounceSound);
	for (int i = 1; i <= indexWalls; i++)
	{
		DoOutsideWallCollision(walls[i], wallBounceSound);
	}

	if(lifeTime <= 0.0f)
	{
		Destroy();
	}
}

void  Bullet::DoWallCollision( const RectF & walls, Sound& wallBounceSound)
{
	const RectF rect = GetRect();
	if( rect.left < walls.left ) 
	{
		pos.x += walls.left - rect.left;
		ReboundX(); 
		wallBounceSound.Play(1.0f, 0.3f);
	}
	else if( rect.right > walls.right )
	{
		pos.x -= rect.right - walls.right;
		ReboundX();
		wallBounceSound.Play(1.0f, 0.3f);
	}
	if( rect.top < walls.top )
	{
		pos.y += walls.top - rect.top;
		ReboundY();
		wallBounceSound.Play(1.0f, 0.3f);
	}
	else if( rect.bottom > walls.bottom )
	{
		pos.y -= rect.bottom - walls.bottom;
		ReboundY();
		wallBounceSound.Play(1.0f, 0.3f);
	}
}

void Bullet::DoOutsideWallCollision(const RectF & wall, Sound& wallBounceSound)
{
	const RectF rect = GetRect();
	if (rect.IsContainedBy(wall))
	{
		Destroy();
	}
	else
	{
		if (rect.IsOverlappingWith(wall))
		{
			wallBounceSound.Play(1.0f,0.3f);
			if (std::signbit(vel.x) == std::signbit((pos - wall.GetCenter()).x))
			{
				ReboundY();
			}
			else
			{
				if (pos.x > wall.left && pos.x < wall.right) {
					ReboundY();
				}
				else {
					ReboundX();
				}
			}
		}
	}
}

void Bullet::ReboundX()
{
	vel.x = -vel.x;
}

void Bullet::ReboundY()
{
	vel.y = -vel.y;
}

RectF Bullet::GetRect() const
{
	return RectF::FromCenter( pos,radius,radius );
}

Vec2 Bullet::GetVelocity() const
{
	return vel;
}

Vec2 Bullet::GetPosition() const
{
	return pos;
}

void Bullet::SetDirection( const Vec2 & dir )
{
	vel = dir.GetNormalized() * speed;
}

void Bullet::Load()
{
	loaded = true;
}

bool Bullet::IsLoaded() const
{
	return loaded;
}

bool Bullet::IsSpawned() const
{
	return spawned;
}

float Bullet::GetLifeTime() const
{
	return lifeTime;
}

void Bullet::Spawn(const Vec2 & pos_in, const Vec2 & dir_in, float lifetime_in)
{
	spawned = true;
	loaded = false;
	pos = pos_in;
	vel = dir_in * speed;
	lifeTime = lifetime_in;
}

void Bullet::Destroy()
{
	spawned = false;
}

void Bullet::LowerLifeTime(float dt)
{
	lifeTime -= dt;
}
