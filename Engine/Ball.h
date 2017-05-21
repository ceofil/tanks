#pragma once

#include "Vec2.h"
#include "Graphics.h"
#include "RectF.h"
#include "SpriteCodex.h"
#include <chrono>

class Ball
{
public:
	Ball() = default;
	Ball( const Vec2& pos_in,const Vec2& dir_in );
	void Draw( Graphics& gfx ) const;
	void Update(float dt, RectF walls[], int indexWalls);
	// return: 0=nada 1=hit wall 2=hit bottom
	void DoWallCollision( const RectF& walls );
	void DoOutsideWallCollision(const RectF& wall);
	void ReboundX();
	void ReboundY();
	RectF GetRect() const;
	Vec2 GetVelocity() const;
	Vec2 GetPosition() const;
	void SetDirection( const Vec2& dir );
	bool IsSpawned();
	void Spawn(const Vec2& pos_in,const Vec2& dir_in, float lifetime_in);
	void Destroy();
	static constexpr float radius = 7.0f;
private:
	float lifeTime = 0.0f;
	bool spawned = false;
	float speed = 275.0f;
	Vec2 pos;
	Vec2 vel;
};