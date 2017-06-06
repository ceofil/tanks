#include "ElectricField.h"

ElectricField::ElectricField(Vec2 in_pos, float in_radius)
	:
	pos(in_pos),
	startRadius(in_radius),
	rng(rd()),
	xRange(0, Graphics::ScreenWidth - 1),
	yRange(0, Graphics::ScreenHeight - 1),
	pixelVariation(-1,1),
	lengthRange(50, 500),
	binary(0, 1)
{
	Reset();
	intPos = Vei2(int(pos.x), int(pos.y));
}

void ElectricField::Draw(Graphics & gfx)
{
	gfx.DrawCircle(pos, radius, safezone);

	for (int i = 0; i < numberOfElectricThings; i++)
	{
		if ((things[i].point - intPos).GetLength() > radius )
		{
			int len = lengthRange(rng);
			things[i].Draw(gfx, len);
		}
	}
}

void ElectricField::DrawCircle(Graphics & gfx)
{
	gfx.DrawCircleStrokeOnly(pos, radius, 5.0f, circleColor);
}

void ElectricField::Update(float dt)
{
	if (radius > minValueRadius)
	{
		const float contractionSpeed = sqrt(radius) * 0.25f;
		radius -= dt *contractionSpeed;
	}
}

void ElectricField::Reset()
{
	radius = startRadius;
	for (int i = 0; i < numberOfElectricThings; i++)
	{
		//things[i] = Vei2(binary(rng)*(Graphics::ScreenWidth - 1), binary(rng)*(Graphics::ScreenHeight - 1));
		things[i].point = Vei2( xRange(rng), yRange(rng)  );
	}
}

void ElectricField::ElectricThing::Draw(Graphics& gfx, int length)
{
	if (length > 0)
	{
		
		Vei2 delta = Vei2(pixelVariation(rng), pixelVariation(rng));
		while (!validPoint(point + delta))
		{
			delta = Vei2(pixelVariation(rng), pixelVariation(rng));
		}


		gfx.PutPixel(point.x, point.y, electricThing);
		point += delta;
		
		Draw(gfx, length - 1);
	}
}

bool ElectricField::ElectricThing::validPoint(Vei2 in_point) const
{
	return	in_point.x >= 0 && in_point.y >= 0 && in_point.x < Graphics::ScreenWidth && in_point.y < Graphics::ScreenHeight &&
			(intPos - in_point).GetLength() >= GetRadius();
}

Vec2 ElectricField::GetPos() const
{
	return pos;
}

float ElectricField::GetRadius() const
{
	return radius;
}
