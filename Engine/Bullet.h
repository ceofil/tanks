#pragma once

#include "Vec2.h"
#include "Graphics.h"
#include "RectF.h"
#include "SpriteCodex.h"
#include <chrono>
#include "Sound.h"

class Bullet
{
public:
	Bullet() = default;
	Bullet( const Vec2& pos_in,const Vec2& dir_in );
	void Draw( Graphics& gfx ) const;
	void Update(float dt, RectF walls[], int indexWalls, Sound& wallBounceSound);
	void DoWallCollision( const RectF& walls, Sound& wallBounceSound);
	void DoOutsideWallCollision(const RectF& wall, Sound& wallBounceSound);
	void ReboundX();
	void ReboundY();
	void Spawn(const Vec2& pos_in, const Vec2& dir_in, float lifetime_in);
	void Destroy();
	void LowerLifeTime(float dt);
public:
	void SetDirection(const Vec2& dir);
	void Load();
	RectF GetRect() const;
	Vec2 GetVelocity() const;
	Vec2 GetPosition() const;
	bool IsSpawned() const;
	bool IsLoaded() const;
	float GetLifeTime() const;
	static constexpr float radius = 2.5f;
private:
	bool spawned = false;
	bool loaded = true;
	float lifeTime = 0.0f;
	float speed = 1200.0f;
	Vec2 pos;
	Vec2 vel;
};