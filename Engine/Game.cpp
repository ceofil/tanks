/******************************************************************************************
 *	Chili DirectX Framework Version 16.07.20											  *
 *	Game.cpp																			  *
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
#include "MainWindow.h"
#include "Game.h"
#include <cmath>
#include <random>
#include <chrono>

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	txt(gfx,0,0,4,4,800,600),
	p1(Vec2(450.0f, 300.0f), 0.0f, Color(255, 80, 80)),
	p2(Vec2(350.0f, 300.0f), 180.0f, Color(51, 204, 51))
{
}

void Game::Go()
{
	gfx.BeginFrame();
	float timeperiod = ft.Mark();
	while (timeperiod > 0.0f)
	{
		const float dt = std::min(0.0025f, timeperiod);
		UpdateModel(dt);
		timeperiod -= dt;
	}

	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel(float dt)
{
	if (!gameIsStarted)
	{
		CreateWalls();
		
		if (wnd.kbd.KeyIsPressed(VK_RETURN))
		{
			gameIsStarted = true;
		}
	}
	else 
	{
		if (wnd.kbd.KeyIsPressed(VK_ESCAPE))
		{
			gameIsStarted = false;
		}
		p1.Update(wnd.kbd, dt, walls, indexWalls, p2, balls, nBalls, VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT);
		p2.Update(wnd.kbd, dt, walls, indexWalls, p1, balls, nBalls, 0x57, 0x53, 0x41, 0x44);


		if (!wnd.kbd.KeyIsEmpty())
		{
			const auto e = wnd.kbd.ReadKey();
			if (e.IsRelease())
			{
				if (e.GetCode() == VK_CONTROL)
				{
					Player1_Shoot();
				}
				if (e.GetCode() == VK_SPACE)
				{
					Player2_Shoot();
				}
				
			}
		}
		UpdateBalls(dt);
	}
}



void Game::ComposeFrame()
{
	gfx.FillScreenWith(Color(50, 50, 50));
	DrawWalls();
	p1.Draw(gfx);
	p2.Draw(gfx);
	if (gameIsStarted)
	{
		DrawScore();
		DrawBalls();
	}
}





void Game::CreateWalls()
{
	if (wnd.mouse.LeftIsPressed())
	{
		if (!RectStarted)
		{
			RectStarted = true;
			x1 = x2 = wnd.mouse.GetPosX();
			y1 = y2 = wnd.mouse.GetPosY();
			indexWalls++;
		}
		else
		{
			x2 = wnd.mouse.GetPosX();
			y2 = wnd.mouse.GetPosY();
		}

		walls[indexWalls] = RectF(float(std::min(x1, x2)), float(std::max(x1, x2)), float(std::min(y1, y2)), float(std::max(y1, y2)));
	}
	else if (RectStarted)
	{
		RectStarted = false;
		if (indexWalls >= nWalls - 1)
		{
			gameIsStarted = true;
		}
	}
	
	
	if (wnd.kbd.KeyIsPressed(VK_CONTROL))
	{
		const auto e = wnd.kbd.ReadKey();
		if (e.IsRelease())
		{
			if (e.GetCode() == 0x5A)
			{
				if (indexWalls >= 0)
				{
					indexWalls--;
				}
			}
		}
	}
	else if (!wnd.kbd.KeyIsEmpty())
	{
		const auto e = wnd.kbd.ReadKey();
		if (e.IsRelease())
		{
			if (e.GetCode() == VK_SPACE)
			{
				if (!standardApplied)
				{
					standardApplied = true;
					walls[++indexWalls] = RectF(150.0f, float(Graphics::ScreenWidth) - 150.0f, 125.0f, 175.0f);
					walls[++indexWalls] = RectF(150.0f, float(Graphics::ScreenWidth) - 150.0f, float(Graphics::ScreenHeight) - 175.0f, float(Graphics::ScreenHeight) - 125.0f);
				}
			}

			if (e.GetCode() == VK_RETURN)
			{
				gameIsStarted = true;
			}
		}
	}
}

void Game::Player1_Shoot()
{
	bool test = true;
	Vec2 spawnPoint = p1.GetSpawnPoint();
	for (int i = 0; i < indexWalls; i++)
	{
		if (walls[i].ContainsPoint(spawnPoint))
		{
			test = false;
			break;
		}
	}
	if (test)
	{
		for (int i = 0; i < nBalls / 2; i++)
		{
			if (balls[i].IsSpawned() == false)
			{
				balls[i].Spawn(p1.GetSpawnPoint(), p1.GetDir(), 7.0f);
				break;
			}
		}
	}
}

void Game::Player2_Shoot()
{
	bool test = true;
	Vec2 spawnPoint = p1.GetSpawnPoint();
	for (int i = 0; i < indexWalls; i++)
	{
		if (walls[i].ContainsPoint(spawnPoint))
		{
			test = false;
			break;
		}
	}
	if (test)
	{
		for (int i = nBalls / 2; i < nBalls; i++)
		{
			if (balls[i].IsSpawned() == false)
			{
				balls[i].Spawn(p2.GetSpawnPoint(), p2.GetDir(), 7.0f);
				break;
			}
		}
	}
}

void Game::UpdateBalls(float dt)
{
	for (int i = 0; i < nBalls; i++)
	{
		if (balls[i].IsSpawned() == true)
		{
			balls[i].Update(dt, walls, indexWalls);
		}
	}
}

void Game::DrawBalls()
{
	for (int i = 0; i < nBalls; i++)
	{
		if (balls[i].IsSpawned())
		{
			balls[i].Draw(gfx);
		}
	}
}

void Game::DrawWalls()
{
	for (int i = 0; i <= indexWalls; i++)
	{
		gfx.DrawRectPoints(walls[i], Color(100,100,100));
	}
}

void Game::DrawScore()
{
	const int sw = Graphics::ScreenWidth;
	const int sh = Graphics::ScreenHeight;
	const int hSpacing = 10;
	const int wSpacing = 50;
	const int height = 20;
	
	const int width2 = p1.GetHP() * 285 / 10;
	gfx.DrawRectPoints(sw - wSpacing - width2, sh - hSpacing - height, sw - wSpacing, sh - hSpacing, p1.GetColor());
	gfx.DrawRectStrokeOnly(sw - wSpacing - 285, sh - hSpacing - height, sw - wSpacing, sh - hSpacing, p1.GetColor());
	txt.drawint(p1.GetHP(), (sw - wSpacing)/4 + 3, (sh - hSpacing) / 4 - 6, Colors::White);

	const int width1 = p2.GetHP() * 285 / 10;
	gfx.DrawRectPoints(wSpacing, sh - hSpacing - height, wSpacing + width1, sh - hSpacing, p2.GetColor());
	gfx.DrawRectStrokeOnly(wSpacing, sh - hSpacing - height, wSpacing + 285, sh - hSpacing, p2.GetColor());
	txt.drawintRight(p2.GetHP(), wSpacing / 4 - 1, (sh - hSpacing) / 4 - 6, Colors::White);

	gfx.DrawRectPoints(345, sh - hSpacing - height - 11, sw - 345, sh - 3, Color(80, 80, 80));
	txt.drawint(p2.GetScore(), (wSpacing + 300) / 4 + 1, (sh - hSpacing) / 4 - 6, Colors::White);
	txt.drawintRight(p1.GetScore(), (sw - wSpacing - 300) / 4 + 1, (sh - hSpacing) / 4 - 6, Colors::White);
}
