#pragma once
#include "Vec2.h"
#include "Sound.h"
#include "Graphics.h"

class ElectricField
{
public:
	ElectricField(Vec2 in_pos, float in_radius);
	void Draw(Graphics& gfx);
	void Update(float dt);
	void Reset();
public:
	Vec2 GetPos()const;
	float GetRadius()const;
private:
	Vec2 pos;
	float startRadius;
	float radius;
public:
};