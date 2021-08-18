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
#include "Mat.h"

//#include "CubeSceneTex.h"
//#include "CubeSceneColorBlend.h"
//#include "CubeSceneSolid.h"
//#include "DoubleCubeScene.h"
//#include "PlaneWaveScene.h"
//#include "CubeSceneVertexColor.h"
//#include "CubeSceneSolidGeometry.h"
//#include "FlatVertexScene.h"
//#include "FlatGeometryScene.h"
//#include "Sphere.h"
//#include "GouraudScene.h"
//#include "GouraudPointScene.h"
//#include "PhongPointScene.h"
#include "SpecularPhongScene.h"
#include "Cube.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
	/*scenes.emplace_back( std::make_unique<CubeSceneTex>( gfx, L"WoodTex.jpg" ) );
	scenes.emplace_back( std::make_unique<CubeSceneColorBlend>( gfx ) );
	scenes.emplace_back( std::make_unique<CubeSceneSolid>( gfx ) );
	scenes.emplace_back( std::make_unique<DoubleCubeScene>( gfx ) );
	scenes.emplace_back( std::make_unique<PlaneWaveScene>( gfx ) );
	scenes.emplace_back( std::make_unique<CubeSceneVertexColor>( gfx ) );
	scenes.emplace_back( std::make_unique<CubeSceneSolidGeometry>( gfx ) );
	scenes.emplace_back( std::make_unique<FlatVertexScene>( gfx ) );
	scenes.emplace_back( std::make_unique<FlatGeometryScene>( gfx, IndexedTriangleList<FlatGeometryEffect::Vertex>::LoadFromFile( "bunny.obj" )));
	scenes.emplace_back( std::make_unique<FlatGeometryScene>( gfx, Sphere::GetPlain<FlatGeometryEffect::Vertex>( 1.0f, 40u, 40u ) ) );
	scenes.emplace_back( std::make_unique<GouraudScene>( gfx, Sphere::GetNormals<GouraudEffect::Vertex>( 1.0f, 40u, 40u ) ) );
	scenes.emplace_back( std::make_unique<GouraudScene>( gfx, IndexedTriangleList<GouraudEffect::Vertex>::LoadNormals("suzanne.obj")));
	scenes.emplace_back( std::make_unique<GouraudPointScene>( gfx, Sphere::GetNormals<GouraudPointEffect::Vertex>( 1.0f, 40, 40 ) ) );
	scenes.emplace_back( std::make_unique<GouraudPointScene>( gfx, Cube::GetPlainIndependentFacesNormals<GouraudPointEffect::Vertex>() ) );
	scenes.emplace_back( std::make_unique<PhongPointScene>( gfx, Cube::GetPlainIndependentFacesNormals<PhongPointEffect::Vertex>() ) );
	scenes.emplace_back( std::make_unique<PhongPointScene>( gfx, Plane::GetNormals<PhongPointEffect::Vertex>(  ) ) );*/
	scenes.emplace_back( std::make_unique<SpecularPhongScene>( gfx, Sphere::GetNormals<SpecularPhongEffect::Vertex>(1.0f, 40, 40) ) );
	scenes.emplace_back( std::make_unique<SpecularPhongScene>( gfx, IndexedTriangleList<SpecularPhongEffect::Vertex>::LoadNormals("suzanne.obj") ) );
	scenes.emplace_back( std::make_unique<SpecularPhongScene>(gfx, Cube::GetPlainIndependentFacesNormals<SpecularPhongEffect::Vertex>()) );

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
	(*i)->Render();
}