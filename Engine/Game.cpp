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

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	c( 1.0f )
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
	const float dt = 1 / 60.0f;
	const float dTheta = PI / 2;

	if ( wnd.kbd.KeyIsPressed(VK_UP) )
	{
		zOffset += 0.1f;
	}
	else if ( wnd.kbd.KeyIsPressed(VK_DOWN) )
	{
		zOffset -= 0.1f;
	}

	if ( wnd.kbd.KeyIsPressed('Q') )
	{
		theta_z += dt * dTheta;
	}
	else if ( wnd.kbd.KeyIsPressed('E') )
	{
		theta_z -= dt * dTheta;
	}
	
	if ( wnd.kbd.KeyIsPressed('W') )
	{
		theta_x += dt * dTheta;
	}
	else if ( wnd.kbd.KeyIsPressed('S') )
	{
		theta_x -= dt * dTheta;
	}
	
	if ( wnd.kbd.KeyIsPressed('A') )
	{
		theta_y += dt * dTheta;
	}
	else if ( wnd.kbd.KeyIsPressed('D') )
	{
		theta_y -= dt * dTheta;
	}
	theta_x = wrap_angle( theta_x );
	theta_y = wrap_angle( theta_y );
	theta_z = wrap_angle( theta_z );
}

void Game::ComposeFrame()
{
	// the colors of the sides of the cube
	std::vector<Color> colors = {
		Colors::Yellow,
		Colors::Red,
		Colors::Blue,
		Colors::White,
		Colors::Green,
		Colors::Cyan
	};

	// get triangles and transform vertices from model space to world space
	auto triangles = c.GetTriangles();
	const Mat3 m = Mat3::RotationX(theta_x) * Mat3::RotationY(theta_y) * Mat3::RotationZ(theta_z);
	for (auto& v : triangles.vertices)
	{
		v *= m;
		v += { 0.0f, 0.0f, zOffset };
	}

	// cull backfaces
	for ( size_t i = 0, e = triangles.indices.size() / 3; i < e; i++ )
	{
		const Vec3& v0 = triangles.vertices[triangles.indices[i * 3]];
		const Vec3& v1 = triangles.vertices[triangles.indices[i * 3 + 1]];
		const Vec3& v2 = triangles.vertices[triangles.indices[i * 3 + 2]];
		
		triangles.cullingFlags[i] = ( v1 - v0 ) % ( v2 - v0 ) * v0 > 0.0f;
	}

	// transform vertices to screen/graphics space
	for ( auto& v : triangles.vertices )
	{
		ct.Transform( v );
	}

	// draw triangles
	for ( size_t i = 0, e = triangles.indices.size() / 3; i < e; i++ )
	{
		if ( !triangles.cullingFlags[i] ) {
			const Vec3& v0 = triangles.vertices[triangles.indices[i * 3]];
			const Vec3& v1 = triangles.vertices[triangles.indices[i * 3 + 1]];
			const Vec3& v2 = triangles.vertices[triangles.indices[i * 3 + 2]];

			gfx.DrawTriangle( v0, v1, v2, colors[i / 2] );
		}
	}
}