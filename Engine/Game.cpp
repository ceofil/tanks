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
	rng(rd()),
	xrange(0, gfx.ScreenWidth - 30),
	yrange(0, gfx.ScreenHeight - 30),
	rgbrange(0,255),
	nr(gfx,25,50,15,15,50,7),
	text(gfx, 25, 300, 7, 7, 50, 7)
{	
}

void Game::Go()
{
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
}



void Game::ComposeFrame()
{
	/*
	if (kframe == 0) {
		start = std::chrono::steady_clock::now();
	}
	kframe++;
	if (kframe >= 60) {
		end = std::chrono::steady_clock::now();

		std::chrono::duration<float> runtime = end - start;
		durationSecond = runtime.count();

		kframe = 0;


		
	}
	text.drawfloat(durationSecond, 5, 5, Colors::White);
	text.drawint(kframe, 5, 15, Colors::White);
	*/

	if (durationSecond == 0.0f) {
		start = std::chrono::steady_clock::now();
	}
	end = std::chrono::steady_clock::now();
	std::chrono::duration<float> runtime = end - start;
	durationSecond = runtime.count();
	kframe++;
	if (durationSecond >= 1.0f) {
		durationSecond = 0.0f;
		k = kframe;
		kframe = 0;
		gfx.DrawCircle(gfx.ScreenWidth / 2, gfx.ScreenHeight / 2, 200, Colors::Red);
	}
	text.drawfloat(durationSecond, 5, 5, Colors::White);
	text.drawint(kframe, 5, 15, Colors::White);


	text.drawint(k,5, 25, Colors::White);
}




