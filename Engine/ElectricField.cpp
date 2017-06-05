#include "ElectricField.h"

ElectricField::ElectricField(Vec2 in_pos, float in_radius)
	:
	pos(in_pos),
	startRadius(in_radius),
	rng(rd()),
	pixelVariation(-1, 1),
	xRange(0, Graphics::ScreenWidth - 1),
	yRange(0, Graphics::ScreenHeight - 1),
	lengthRange(50, 500),
	binary(0, 1)
{
	Reset();
	intPos = Vei2(int(pos.x), int(pos.y));
}

void ElectricField::Draw(Graphics & gfx)
{
	int nrThings = int(float(numberOfElectricThings) *  float((Graphics::ScreenWidth + minValueRadius - int(radius))) / float(Graphics::ScreenWidth));
	for (int i = 0; i < nrThings; i++)
	{
		if ((things[i] - intPos).GetLength() > radius )
		{
			int len = lengthRange(rng);
			DrawElectricThing(gfx, things[i], len, i);
		}
	}
}

void ElectricField::DrawCircle(Graphics & gfx)
{
	gfx.DrawCircleStrokeOnly(pos, radius, 5.0f, cl);
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
		things[i] = Vei2( xRange(rng), yRange(rng)  );
	}
}

void ElectricField::DrawElectricThing(Graphics& gfx, Vei2 point,  int length, int index)
{
	if (length > 0)
	{
		if(gfx.insideScreen(point.x,point.y))
		{
			gfx.PutPixel(point.x, point.y, electricThing);
		}

		Vei2 delta = Vei2(pixelVariation(rng), pixelVariation(rng));

		while (!validPoint(point + delta))
		{
			delta = Vei2(pixelVariation(rng), pixelVariation(rng));
		}
		DrawElectricThing(gfx, point + delta, length - 1, index);
	}
	else
	{
		things[index] = point;
	}
}

bool ElectricField::validPoint(Vei2 point)
{
	return	point.x >= 0 && point.y >= 0 && point.x < Graphics::ScreenWidth && point.y < Graphics::ScreenHeight &&
			(intPos - point).GetLength() >= radius;
}

Vec2 ElectricField::GetPos() const
{
	return pos;
}

float ElectricField::GetRadius() const
{
	return radius;
}
