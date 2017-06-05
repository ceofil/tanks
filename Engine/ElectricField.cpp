#include "ElectricField.h"

ElectricField::ElectricField(Vec2 in_pos, float in_radius)
	:
	pos(in_pos),
	startRadius(in_radius)
{
	Reset();
}

void ElectricField::Draw(Graphics & gfx)
{
	gfx.DrawCircleStrokeOnly(pos, radius, 5.0f, Color(0, 100, 255));
}

void ElectricField::Update(float dt)
{
	if (radius > 100.0f)
	{
		const float contractionSpeed = sqrt(radius) * 0.18f;
		radius -= dt *contractionSpeed;
	}
}

void ElectricField::Reset()
{
	radius = startRadius;
}

Vec2 ElectricField::GetPos() const
{
	return pos;
}

float ElectricField::GetRadius() const
{
	return radius;
}
