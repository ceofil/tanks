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
	p1(Vec2(400.0f, 400.0f), 45.0f),
	p2(Vec2(400.0f, 450.0f), 135.0f)
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
		p1.Update(wnd.kbd, dt, walls, indexWalls, p2, balls, nBalls, VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT);
		p2.Update(wnd.kbd, dt, walls, indexWalls, p1, balls, nBalls, 0x57, 0x53, 0x41, 0x44);


		if (!wnd.kbd.KeyIsEmpty())
		{
			const auto e = wnd.kbd.ReadKey();
			if (e.IsRelease())
			{
				if (e.GetCode() == VK_SPACE)
				{
					Player1_Shoot();
				}
				if (e.GetCode() == VK_CONTROL)
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
	DrawWalls();
	if (gameIsStarted)
	{
		DrawBalls();
		p1.Draw(gfx);
		p2.Draw(gfx);
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
		}
		else
		{
			x2 = wnd.mouse.GetPosX();
			y2 = wnd.mouse.GetPosY();
		}

		RectF rect = RectF(float(std::min(x1, x2)), float(std::max(x1, x2)), float(std::min(y1, y2)), float(std::max(y1, y2)));
		gfx.DrawRectPoints(rect, Colors::Yellow);
	}
	else if (RectStarted)
	{
		RectStarted = false;
		walls[++indexWalls] = RectF(float(std::min(x1, x2)), float(std::max(x1, x2)), float(std::min(y1, y2)), float(std::max(y1, y2)));
		x1 = x2 = y1 = y2 = 0;
		if (indexWalls >= nWalls - 1)
		{
			gameIsStarted = true;
		}
	}
}

void Game::Player1_Shoot()
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

void Game::Player2_Shoot()
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
		gfx.DrawRectPoints(walls[i], Colors::White);
	}
}
