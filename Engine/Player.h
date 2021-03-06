#pragma once
#include "Graphics.h"
#include "Vec2.h"
#include "Keyboard.h"
#include "RectF.h"
#include "Bullet.h"
#include "Sound.h"
#include "SoundEffect.h"
#include "ElectricField.h"
#include <random>
class Player
{
public:
	Player(const Vec2 in_pos, const float in_angle, Color c, int in_index);
	void Draw(Graphics& gfx) const;
	void Update(Keyboard& kbd, const float dt, 
				RectF walls[], int indexWalls, 
				Player& other, 
				Bullet bullets[], int nBullets,
				ElectricField& field,
				const int up, const int down, const int left, const int right);
public:
	void DoWallCollision(const RectF& wall, const Vec2 dir, const float dt);
	void KeepInsideScreen(const RectF& screen);

	//checks to see if the circle of the player is overlapping with another circle
	bool IsOverLappingWith(const Vec2 other, float r); 
	
	bool IsContainedBy(const Vec2 center, float r);

	//this undoes the movement that caused the players to overlap
	void DoPlayerCollision(Player& other, float dt);
	void DoFieldInteraction(ElectricField& field);
	
	//when one player drops below 1 HP the other player gains a point and everything gets reset
	void NewRound();

	void Move(Vec2 dir_in, float dt);
	void AddToScore();
	void LowerHP(float dmg);
	void Reload(); //this one just starts the timer
	void Reload(Bullet* bullets, int nBullets); //this one actually reloads
public:
	RectF GetRect() const;
	Vec2 GetPos() const;
	Vec2 GetDir() const;
	Vec2 GetSpawnPoint() const;
	int GetScore() const;
	float GetHP() const;
	float GetMaxHP() const;
	Color GetColor() const;
	bool IsReloading() const;
	float GetPercentTimeLeft() const;

private:
	float reloadingTimeLeft = 0.0f;
	static constexpr float reloadingTime = 1.5f;
	static constexpr float speed = 200.0f;
	static constexpr float rotationSpeed = 100.0f;
	static constexpr float radius = 15.0f;
	static constexpr float scopeRadius = 0.2f; //ratio
	static constexpr float pi = 3.14159265f;
	Vec2 AngleToVec2(const float& angle);
	static constexpr float maxHP = 99;
	float HP;
	int score = 0;
	Color c;
private:
	Vec2 pos;
	Vec2 dir;
	float angle;
	int playerIndex;

	//so I have what to set it to in NewRound();
	Vec2 startPos; 
	float startAngle;
	SoundEffect hit;
	SoundEffect electricSound;

	std::random_device rd;
	std::mt19937 rng;
};