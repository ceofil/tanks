#pragma once
#include "Vec2.h"
#include "Vei2.h"
#include "Sound.h"
#include "Graphics.h"
#include <random>

class ElectricField
{
public:
	ElectricField(Vec2 in_pos, float in_radius);
	void Draw(Graphics& gfx);
	void DrawCircle(Graphics& gfx) const;
	void Update(float dt);
	void Reset();

public:
	Vec2 GetPos()const;
	float GetRadius()const;

private:
	Vec2 pos;
	Vei2 intPos;
	float startRadius;
	float radius;
	static constexpr float minValueRadius = 100.0f;

private:
	std::random_device rd;
	std::mt19937 rng;
	std::uniform_int_distribution<int> xRange;
	std::uniform_int_distribution<int> yRange;
	std::uniform_int_distribution<int> pixelVariation;
	std::uniform_int_distribution<int> lengthRange;
	std::uniform_int_distribution<int> binary;

	static constexpr Color circleColor = Color(0, 100, 255);
	static constexpr Color electricThingColor = Color(130, 190, 255);
	static constexpr Color safezoneColor = Color(50, 50, 50);

private:
	struct ElectricThing
	{
		void Draw(ElectricField& field, Graphics& gfx, int length);
		bool validPoint(ElectricField& field, const Vei2 in_point) const;
		Vei2 point;
	};
	static constexpr int numberOfElectricThings = 1000;
	ElectricThing things[numberOfElectricThings];


};