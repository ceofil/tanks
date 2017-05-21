#pragma once
#include "Graphics.h"
#include "Vec2.h"
#include "Keyboard.h"
#include "RectF.h"
#include "Ball.h"
class Player
{
public:
	Player(const Vec2 in_pos, const float in_angle);
	void Draw(Graphics& gfx) const;
	void Update(Keyboard& kbd, const float dt, 
				RectF walls[], int indexWalls, 
				Player& other, 
				Ball balls[], int nBalls,
				const int up, const int down, const int left, const int right);
	RectF GetRect() const;
	void DoWallCollision(const RectF& wall, const Vec2 dir, const float dt);
	Vec2 GetPos() const;
	Vec2 GetDir() const;
	Vec2 GetSpawnPoint() const;
	bool IsOverLappingWith(const Vec2 other, float r);
	void DoPlayerCollision(Player& other, float dt);
	void Move(Vec2 dir_in, float dt);
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