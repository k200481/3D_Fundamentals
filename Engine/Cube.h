#pragma once

#include <vector>
#include "IndexedLineList.h"
#include "IndexedTriangleList.h"
#include "Vec3.h"
#include "TexVertex.h"

class Cube
{
public:
	Cube( float size )
		:
		side( size / 2.0f )
	{
		vertices.emplace_back( -side,  side, -side );
		vertices.emplace_back( -side,  side,  side );
		vertices.emplace_back(  side,  side,  side );
		vertices.emplace_back(  side,  side, -side );
		vertices.emplace_back( -side, -side, -side );
		vertices.emplace_back( -side, -side,  side );
		vertices.emplace_back(  side, -side,  side );
		vertices.emplace_back(  side, -side, -side );
	}

	IndexedLineList GetLineList() const
	{
		return {
			vertices, {
				0,1 , 1,2 , 2,3 , 3,0,
				0,4 , 1,5 , 2,6 , 3,7,
				4,5 , 5,6 , 6,7 , 7,4
			}
		};
	}
	IndexedTriangleList<Vec3> GetTriangles() const
	{
		return {
			vertices, {
				0,1,2 , 2,3,0,
				4,6,5 , 4,7,6,
				0,3,7 , 7,4,0,
				1,5,2 , 2,5,6,
				0,4,1 , 1,4,5,
				3,2,7 , 2,6,7
			}
		};
	}
	IndexedTriangleList<TexVertex> GetTrianglesTex() const
	{
		std::vector<TexVertex> verts;
		verts.emplace_back( Vec3{ -side,  side, -side }, Vec2{ 0.0f, 0.0f } ); // 0
		verts.emplace_back( Vec3{ -side,  side,  side }, Vec2{ 1.0f, 0.0f } ); // 1
		verts.emplace_back( Vec3{  side,  side,  side }, Vec2{ 0.0f, 0.0f } ); // 2
		verts.emplace_back( Vec3{  side,  side, -side }, Vec2{ 1.0f, 0.0f } ); // 3
		verts.emplace_back( Vec3{ -side, -side, -side }, Vec2{ 0.0f, 1.0f } ); // 4
		verts.emplace_back( Vec3{ -side, -side,  side }, Vec2{ 1.0f, 1.0f } ); // 5
		verts.emplace_back( Vec3{  side, -side,  side }, Vec2{ 0.0f, 1.0f } ); // 6
		verts.emplace_back( Vec3{  side, -side, -side }, Vec2{ 1.0f, 1.0f } ); // 7

		return {
			verts, {
				0,1,2 , 2,3,0,
				4,6,5 , 4,7,6,
				0,3,7 , 7,4,0,
				1,5,2 , 2,5,6,
				0,4,1 , 1,4,5,
				3,2,7 , 2,6,7
			}
		};
	}

private:
	std::vector<Vec3> vertices;
	const float side;
};