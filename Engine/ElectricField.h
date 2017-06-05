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
	void DrawCircle(Graphics& gfx);
	void Update(float dt);
	void Reset();
	void DrawElectricThing(Graphics& gfx, Vei2 point, int length, int index);
	bool validPoint(Vei2 point);
public:
	Vec2 GetPos()const;
	float GetRadius()const;
private:
	Vec2 pos;
	Vei2 intPos;
	float startRadius;
	float radius;
	static constexpr float minValueRadius = 100.0f;

	std::random_device rd;
	std::mt19937 rng;
	std::uniform_int_distribution<int> pixelVariation;
	std::uniform_int_distribution<int> xRange;
	std::uniform_int_distribution<int> yRange;
	std::uniform_int_distribution<int> lengthRange;
	std::uniform_int_distribution<int> binary;

	static constexpr Color cl = Color(0, 100, 255);
	static constexpr Color electricThing = Color(130, 190, 255);


	static constexpr int numberOfElectricThings = 1000;
	Vei2 things[numberOfElectricThings];
};