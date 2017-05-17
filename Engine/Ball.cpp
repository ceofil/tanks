#include "Ball.h"

Ball::Ball( const Vec2 & pos_in,const Vec2 & dir_in )
	:
	pos( pos_in )
{
	SetDirection( dir_in );
}

void Ball::Draw( Graphics & gfx ) const
{
	SpriteCodex::DrawBall( pos,gfx );
}

void Ball::Update( float dt )
{
	pos += vel * dt;
	DoWallCollision(RectF(Vec2(0.0f, 0.0f), Vec2(Graphics::ScreenWidth, Graphics::ScreenHeight)));
}

// return: 0=nada 1=hit wall 2=hit bottom
void  Ball::DoWallCollision( const RectF & walls )
{
	const RectF rect = GetRect();
	if( rect.left < walls.left )
	{
		pos.x += walls.left - rect.left;
		ReboundX();
	}
	else if( rect.right > walls.right )
	{
		pos.x -= rect.right - walls.right;
		ReboundX();
	}
	if( rect.top < walls.top )
	{
		pos.y += walls.top - rect.top;
		ReboundY();
	}
	else if( rect.bottom > walls.bottom )
	{
		pos.y -= rect.bottom - walls.bottom;
		ReboundY();
	}
}

void Ball::DoOutsideWallCollision(const RectF & wall)
{
	const RectF rect = GetRect();
	if (rect.IsOverlappingWith(wall))
	{
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

void Ball::ReboundX()
{
	vel.x = -vel.x;
}

void Ball::ReboundY()
{
	vel.y = -vel.y;
}

RectF Ball::GetRect() const
{
	return RectF::FromCenter( pos,radius,radius );
}

Vec2 Ball::GetVelocity() const
{
	return vel;
}

Vec2 Ball::GetPosition() const
{
	return pos;
}

void Ball::SetDirection( const Vec2 & dir )
{
	vel = dir.GetNormalized() * speed;
}
