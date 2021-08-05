#pragma once

#include <vector>
#include "IndexedLineList.h"
#include "IndexedTriangleList.h"
#include "Vec3.h"
#include "Pipeline.h"

class Cube
{
	typedef Pipeline::Vertex Vertex;
public:
	Cube( float size, float texDim = 1.0f )
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
	IndexedTriangleList<Vertex> GetTrianglesTex( float texDim = 1.0f ) const
	{
		std::vector<Vertex> verts;
		verts.emplace_back( Vec3{ -side,  side, -side }, Vec2{ 0.0f, 0.0f } ); // 0
		verts.emplace_back( Vec3{ -side,  side,  side }, Vec2{ texDim, 0.0f } ); // 1
		verts.emplace_back( Vec3{  side,  side,  side }, Vec2{ 0.0f, 0.0f } ); // 2
		verts.emplace_back( Vec3{  side,  side, -side }, Vec2{ texDim, 0.0f } ); // 3
		verts.emplace_back( Vec3{ -side, -side, -side }, Vec2{ 0.0f, texDim } ); // 4
		verts.emplace_back( Vec3{ -side, -side,  side }, Vec2{ texDim, texDim } ); // 5
		verts.emplace_back( Vec3{  side, -side,  side }, Vec2{ 0.0f, texDim } ); // 6
		verts.emplace_back( Vec3{  side, -side, -side }, Vec2{ texDim, texDim } ); // 7

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

	IndexedTriangleList<Vertex> GetTrianglesTexUnfolded( float tDim = 1.0f ) const
	{
		std::vector<Vertex> verts;
		verts.emplace_back( Vec3{ -side,  side, -side }, Vec2{ tDim, 0.0f } ); // 0
		verts.emplace_back( Vec3{ -side,  side,  side }, Vec2{ 0.0f, 0.0f } ); // 1
		verts.emplace_back( Vec3{  side,  side,  side }, Vec2{ 3 * tDim, 0.0f } ); // 2
		verts.emplace_back( Vec3{  side,  side, -side }, Vec2{ 2 * tDim, 0.0f } ); // 3
		verts.emplace_back( Vec3{ -side, -side, -side }, Vec2{ tDim, tDim } ); // 4
		verts.emplace_back( Vec3{ -side, -side,  side }, Vec2{ 0.0f, tDim } ); // 5
		verts.emplace_back( Vec3{  side, -side,  side }, Vec2{ 3 * tDim, tDim } ); // 6
		verts.emplace_back( Vec3{  side, -side, -side }, Vec2{ 2 * tDim, tDim } ); // 7
		verts.emplace_back( Vec3{ -side,  side,  side }, Vec2{ 4 * tDim, 0.0f } ); // 8
		verts.emplace_back( Vec3{ -side, -side,  side }, Vec2{ 4 * tDim, tDim } ); // 9
		verts.emplace_back( Vec3{ -side,  side,  side }, Vec2{ tDim, tDim } ); // 10
		verts.emplace_back( Vec3{  side,  side,  side }, Vec2{ 2 * tDim, tDim } ); // 11
		verts.emplace_back( Vec3{ -side, -side,  side }, Vec2{ tDim, 2 * tDim } ); // 12
		verts.emplace_back( Vec3{  side, -side,  side }, Vec2{ 2 * tDim, 2 * tDim } ); // 13

		return {
			verts, {
				0,3,7 , 0,7,4,
				3,2,6 , 3,6,7,
				2,8,9 , 2,9,6,
				1,0,4 , 1,4,5,
				10,11,3 , 10,3,0,
				4,7,13 , 4,13,12
			}
		};

	}

private:
	std::vector<Vec3> vertices;
	const float side;
};