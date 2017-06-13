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
	p1(Vec2(float(Graphics::ScreenWidth / 2)+50.0f, float(Graphics::ScreenHeight / 2)), 0.0f, Color(255, 80, 80), 1),
	p2(Vec2(float(Graphics::ScreenWidth / 2)-50.0f, float(Graphics::ScreenHeight / 2)), 180.0f, Color(51, 204, 51), 2),
	field(Vec2(400.0f,300.0f),1000.0f),
	wallBounceSound(L"ballBounce.wav"),
	bulletShot(L"bulletShot.wav"),
	rng(rd())
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
		p1.Update(wnd.kbd, dt, walls, indexWalls, p2, bullets, nBullets, field, VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT);
		p2.Update(wnd.kbd, dt, walls, indexWalls, p1, bullets, nBullets, field, 0x57, 0x53, 0x41, 0x44);  //w,a,s,d

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
				if (e.GetCode() == VK_SHIFT)
				{
					if (!p1.IsReloading() && CountBulletsLeft(1) < nBullets / 2) 
						p1.Reload();
				}
				if (e.GetCode() == 0x52)
				{
					if (!p2.IsReloading() && CountBulletsLeft(2) < nBullets / 2) 
						p2.Reload();  //R
				}
			}
		}
		UpdateBullets(dt);
		field.Update(dt);
	}
}



void Game::ComposeFrame()
{
	gfx.FillScreenWith(Color(65, 65, 65));
	field.Draw(gfx);
	DrawWalls();
	field.DrawCircle(gfx);
	p1.Draw(gfx);
	p2.Draw(gfx);
	if (gameIsStarted)
	{
		DrawInterface();
		DrawBullets();
	}
	else
	{
		txt.drawstringCenter("pause", Graphics::ScreenWidth/2/4, Graphics::ScreenHeight/6/4, Color(200,200,200));
	}
}





void Game::CreateWalls()
{
	//creating RectF's by clicking and releasing
	if (wnd.mouse.LeftIsPressed())
	{
		if (!RectStarted)
		{
			RectStarted = true;
			x1 = x2 = wnd.mouse.GetPosX();
			y1 = y2 = wnd.mouse.GetPosY();
			if(indexWalls + 1 < nWalls)
			{ 
				indexWalls++;
			}
		}
		else
		{
			x2 = wnd.mouse.GetPosX();
			y2 = wnd.mouse.GetPosY();
		}
		//this should be on the else if but I want it to be drawn while it's created
		RectF rect = RectF(float(std::min(x1, x2)), float(std::max(x1, x2)), float(std::min(y1, y2)), float(std::max(y1, y2)));
		//can't create wall if it is overlapping with a player
		if(!rect.IsOverlappingWith(p1.GetRect()) && !rect.IsOverlappingWith(p2.GetRect()) )
		{ 
			walls[indexWalls] = rect;
		}
		
	}
	else if (RectStarted)
	{
		RectStarted = false;
	}
	UndoWall();
}

void Game::Player1_Shoot()
{
	//so Bullets don't spawn inside walls
	bool test = true;
	Vec2 spawnPoint = p1.GetSpawnPoint();
	Vec2 dimension = Vec2(Bullet::radius, Bullet::radius);
	RectF spawnRect = RectF(spawnPoint - dimension, spawnPoint + dimension);
	for (int i = 1; i < indexWalls; i++)
	{
		if (walls[i].IsOverlappingWith(spawnRect))
		{
			test = false;
			break;
		}
	}
	//spawns a ball at the aim point of the player with the direction of the player 
	if (test)
	{
		for (int i = 0; i < nBullets / 2; i++)
		{
			if (bullets[i].IsSpawned() == false && bullets[i].IsLoaded() )
			{
				bullets[i].Spawn(p1.GetSpawnPoint(), p1.GetDir(), 1.0f);
				bulletShot.Play(1.0f, 0.3f);
				break;
			}
		}
	}
}

void Game::Player2_Shoot()
{
	//so Bullets don't spawn inside walls
	bool test = true;
	Vec2 spawnPoint = p2.GetSpawnPoint();
	Vec2 dimension = Vec2(Bullet::radius, Bullet::radius);
	RectF spawnRect = RectF(spawnPoint - dimension, spawnPoint + dimension);
	for (int i = 1; i < indexWalls; i++)
	{
		if (walls[i].IsOverlappingWith(spawnRect))
		{
			test = false;
			break;
		}
	}
	if (test)
	{
		for (int i = nBullets / 2; i < nBullets; i++)
		{
			if (bullets[i].IsSpawned() == false && bullets[i].IsLoaded() )
			{
				bullets[i].Spawn(p2.GetSpawnPoint(), p2.GetDir(), 1.0f);
				bulletShot.Play(1.0f, 0.3f);
				break;
			}
		}
	}
}

void Game::UpdateBullets(float dt)
{
	for (int i = 0; i < nBullets; i++)
	{
		if (bullets[i].IsSpawned() == true)
		{
			bullets[i].Update(dt, walls, indexWalls, wallBounceSound);
		}
		else 
		{
			if (bullets[i].GetLifeTime() > 0.0f)
			{
				bullets[i].LowerLifeTime(dt);
			}
		}
	}
}

void Game::DrawBullets()
{
	for (int i = 0; i < nBullets; i++)
	{
		if (bullets[i].IsSpawned())
		{
			bullets[i].Draw(gfx);
		}
	}
}

void Game::DrawWalls()
{
	for (int i = 1; i <= indexWalls; i++)
	{
		gfx.DrawRectPoints(walls[i], Color(100,100,100));
	}
}

void Game::UndoWall()
{
	//CTRL - Z
	if (wnd.kbd.KeyIsPressed(VK_CONTROL))
	{
		const auto e = wnd.kbd.ReadKey();
		if (e.IsRelease())
		{
			if (e.GetCode() == 0x5A)
			{
				if (indexWalls >= 1)
				{
					indexWalls--;
				}
			}
		}
	}
}

void Game::DrawInterface()
{
	//don't try to understand this...just a bunch of aligning 
	const int sw = Graphics::ScreenWidth;
	const int sh = Graphics::ScreenHeight;
	const int hSpacing = std::max(sh / 90, 1);
	const int wSpacing = std::max(sw / 24, 1);
	const int height = std::max(sh / 45, 1);
	const int hpBarWidth = sw / 3;

	//p1 and p2 scores
	gfx.DrawRectPoints(wSpacing + hpBarWidth + 10, sh - hSpacing - height - 11, sw - (wSpacing + hpBarWidth + 10), sh - 3, Color(80, 80, 80));
	txt.drawint(p2.GetScore(), (wSpacing + hpBarWidth + 10) / 4 + 1, (sh - hSpacing) / 4 - 6, Colors::White);
	txt.drawintRight(p1.GetScore(), (sw - (wSpacing + hpBarWidth + 10)) / 4, (sh - hSpacing) / 4 - 6, Colors::White);

	//p1 HP 
	const int width1 = int(p1.GetHP()) * hpBarWidth / int(p1.GetMaxHP());
	gfx.DrawRectPoints(sw - wSpacing - width1, sh - hSpacing - height, sw - wSpacing, sh - hSpacing, p1.GetColor());
	gfx.DrawRectStrokeOnly(sw - wSpacing - hpBarWidth, sh - hSpacing - height, sw - wSpacing, sh - hSpacing, p1.GetColor());
	txt.drawint(int(p1.GetHP()), (sw - wSpacing)/4 + 3, (sh - hSpacing) / 4 - 6, Colors::White);

	//p2 HP
	const int width2 = int(p2.GetHP()) * hpBarWidth / int(p1.GetMaxHP());
	gfx.DrawRectPoints(wSpacing, sh - hSpacing - height, wSpacing + width2, sh - hSpacing, p2.GetColor());
	gfx.DrawRectStrokeOnly(wSpacing, sh - hSpacing - height, wSpacing + hpBarWidth, sh - hSpacing, p2.GetColor());
	txt.drawintRight(int(p2.GetHP()), wSpacing / 4 - 1, (sh - hSpacing) / 4 - 6, Colors::White);

	//line
	gfx.DrawRectPoints(0, sh - 41, sw - 1, sh - 40, Color(70, 70, 70));

	//bulletCounter
	const int bulletCountWidth = hpBarWidth * 40 / 100;
	const int spacing = std::max(1,bulletCountWidth/50);
	const int bulletWidth = bulletCountWidth / (nBullets / 2) - spacing;
	const int bulletHeight = height * 75 / 100;

	gfx.DrawRectPoints(wSpacing, sh - 41, wSpacing + (nBullets / 2 - 1)*(bulletWidth + spacing), sh - 40, Color(100, 100, 100));
	gfx.DrawRectPoints( sw - wSpacing, sh - 41, sw - wSpacing - (nBullets / 2 - 1)*(bulletWidth + spacing), sh - 40, Color(100, 100, 100));

	int BulletsLeft = CountBulletsLeft(2);
	for (int i = 0; i < BulletsLeft; i++)
	{
		gfx.DrawRectPoints(wSpacing + i * (bulletWidth+spacing), sh - 2*hSpacing - 2* bulletHeight, wSpacing + (i+1)*bulletWidth + i * spacing, sh - 2 * hSpacing - bulletHeight, Colors::LightGray);
	}

	BulletsLeft = CountBulletsLeft(1);
	for (int i = 0; i < BulletsLeft; i++)
	{
		gfx.DrawRectPoints(sw -wSpacing - i * (bulletWidth + spacing), sh - 2 * hSpacing - 2 * bulletHeight, sw - wSpacing - (i + 1)*bulletWidth - i * spacing, sh - 2 * hSpacing - bulletHeight, Colors::LightGray);
	}
}

int Game::CountBulletsLeft(int player)
{
	int counter = 0;
	for (int i = (player - 1) * nBullets / 2; i < player * nBullets / 2; i++)
	{
		if (bullets[i].IsLoaded())
		{
			counter++;
		}
	}
	return counter;
}

