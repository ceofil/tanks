/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.h																				  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include <cmath>
#include <random>
#include "Text.h"
#include <chrono>
#include "FrameTimer.h"
#include "Bullet.h"
#include "Player.h"
#include "RectF.h"
#include "SpriteCodex.h"
#include "Sound.h"
#include "SoundEffect.h"
#include "ElectricField.h"


class Game
{
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();
private:
	void ComposeFrame();
	void UpdateModel(float dt);
	
	/********************************/
	/*  User Functions              */

	void CreateWalls();
	void Player1_Shoot();
	void Player2_Shoot();
	void UpdateBullets(float dt);
	void DrawBullets();
	void DrawWalls();
	void UndoWall();
	void DrawInterface();

	int CountBulletsLeft(int player);
	/********************************/
private:
	MainWindow& wnd;
	Graphics gfx;
	/********************************/
	/*  User Variables              */

	FrameTimer ft;
	Text txt;
	Player p1;
	Player p2;
	ElectricField field;
	Sound wallBounceSound;
	Sound bulletShot;

	static constexpr int nBullets = 50;
	Bullet bullets[nBullets];

	static constexpr int nWalls = 1+50;
	RectF walls[nWalls];

	int indexWalls = 0;
	bool RectStarted = false;
	int x1, x2, y1, y2;

	bool gameIsStarted = false;

	std::random_device rd;
	std::mt19937 rng;
	/********************************/
};