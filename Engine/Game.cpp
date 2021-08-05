/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
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
#include "Mat3.h"

#include "CubeSceneSolid.h"
#include "CubeSceneTex.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
	scenes.emplace_back( std::make_unique<CubeSceneSolid>() );
	scenes.emplace_back( std::make_unique<CubeSceneTex>( L"WoodTex.jpg" ) );
	scenes.emplace_back( std::make_unique<CubeSceneTex>( L"abby.jpg" ) );

	i = scenes.begin();
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
	const float dt = 1 / 60.0f;
	CycleScenes();

	(*i)->UpdateScene( wnd.mouse, wnd.kbd, dt );
}

void Game::CycleScenes()
{
	Mouse::Event e;
	while ( ( e = wnd.mouse.Read() ).GetType() != Mouse::Event::Type::Invalid )
	{
		if ( e.GetType() == Mouse::Event::LPress )
		{
			if (i == scenes.begin())
			{
				i = std::prev(scenes.end());
			}
			else
			{
				i--;
			}
		}
		else if ( e.GetType() == Mouse::Event::RPress )
		{
			i++;
			if (i == scenes.end())
			{
				i = scenes.begin();
			}
		}
	}
}

void Game::ComposeFrame()
{
	(*i)->Render( gfx );
}