#pragma once

#include <vector>
#include "IndexedLineList.h"
#include "IndexedTriangleList.h"
#include "Vec3.h"

class Cube
{
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

	template <typename Vertex>
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

	template <typename Vertex>
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

	template <typename Vertex>
	static IndexedTriangleList<Vertex> GetPlain( float size = 1.0f )
	{
		const float side = size / 2.0f;

		std::vector<Vec3> vertices;
		vertices.emplace_back( -side,  side, -side );
		vertices.emplace_back( -side,  side,  side );
		vertices.emplace_back(  side,  side,  side );
		vertices.emplace_back(  side,  side, -side );
		vertices.emplace_back( -side, -side, -side );
		vertices.emplace_back( -side, -side,  side );
		vertices.emplace_back(  side, -side,  side );
		vertices.emplace_back(  side, -side, -side );

		std::vector<Vertex> verts( vertices.size() );
		for ( size_t i = 0, e = vertices.size(); i < e; i++ )
		{
			verts[i].pos = vertices[i];
		}

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

	template <typename Vertex>
	static IndexedTriangleList<Vertex> GetPlainIndependentFaces( float size = 1.0f )
	{
		const float side = size / 2.0f;

		std::vector<Vec3> vertices;
		
		// front face
		vertices.emplace_back( -side,  side, -side ); // 0
		vertices.emplace_back(  side,  side, -side ); // 3
		vertices.emplace_back(  side, -side, -side ); // 7
		vertices.emplace_back( -side, -side, -side ); // 4

		// back face
		vertices.emplace_back(  side,  side,  side ); // 2
		vertices.emplace_back( -side,  side,  side ); // 1
		vertices.emplace_back( -side, -side,  side ); // 5
		vertices.emplace_back(  side, -side,  side ); // 6

		// right face
		vertices.emplace_back(  side,  side, -side ); // 3
		vertices.emplace_back(  side,  side,  side ); // 2
		vertices.emplace_back(  side, -side,  side ); // 6
		vertices.emplace_back(  side, -side, -side ); // 7

		// left face
		vertices.emplace_back( -side,  side,  side ); // 1
		vertices.emplace_back( -side,  side, -side ); // 0
		vertices.emplace_back( -side, -side, -side ); // 4
		vertices.emplace_back( -side, -side,  side ); // 5

		// top face
		vertices.emplace_back( -side,  side, -side ); // 0
		vertices.emplace_back( -side,  side,  side ); // 1
		vertices.emplace_back(  side,  side,  side ); // 2
		vertices.emplace_back(  side,  side, -side ); // 3

		// bottom face
		vertices.emplace_back( -side, -side, -side ); // 4
		vertices.emplace_back(  side, -side, -side ); // 7
		vertices.emplace_back(  side, -side,  side ); // 6
		vertices.emplace_back( -side, -side,  side ); // 5

		std::vector<Vertex> verts( vertices.size() );
		for ( size_t i = 0, e = vertices.size(); i < e; i++ )
		{
			verts[i].pos = vertices[i];
		}

		return {
			verts, {
				0,1,2 , 0,2,3, // front face
				4,5,6 , 4,6,7, // back face
				8,9,10 , 8,10,11, // right face
				12,13,14 , 12,14,15, // left face
				16,17,18 , 16,18,19, // top face
				20,21,22 , 20,22,23 // bottom face
			}
		};
	}

	template <typename Vertex>
	static IndexedTriangleList<Vertex> GetPlainIndependentFacesNormals( float size = 1.0f )
	{
		const float side = size / 2.0f;

		std::vector<Vertex> vertices( 24 );
		
		// front face
		vertices[0].pos = { -side,  side, -side }; // 0
		vertices[1].pos = {  side,  side, -side }; // 3
		vertices[2].pos = {  side, -side, -side }; // 7
		vertices[3].pos = { -side, -side, -side }; // 4
		vertices[0].n = { 0.0f, 0.0f, -1.0f };
		vertices[1].n = { 0.0f, 0.0f, -1.0f };
		vertices[2].n = { 0.0f, 0.0f, -1.0f };
		vertices[3].n = { 0.0f, 0.0f, -1.0f };

		// back face
		vertices[4].pos = {  side,  side,  side }; // 2
		vertices[5].pos = { -side,  side,  side }; // 1
		vertices[6].pos = { -side, -side,  side }; // 5
		vertices[7].pos = {  side, -side,  side }; // 6
		vertices[4].n = { 0.0f, 0.0f, 1.0f };
		vertices[5].n = { 0.0f, 0.0f, 1.0f };
		vertices[6].n = { 0.0f, 0.0f, 1.0f };
		vertices[7].n = { 0.0f, 0.0f, 1.0f };

		// right face
		vertices[8].pos = {  side,  side, -side }; // 3
		vertices[9].pos = {  side,  side,  side }; // 2
		vertices[10].pos = {  side, -side,  side }; // 6
		vertices[11].pos = {  side, -side, -side }; // 7
		vertices[8].n = { 1.0f, 0.0f, 0.0f };
		vertices[9].n = { 1.0f, 0.0f, 0.0f };
		vertices[10].n = { 1.0f, 0.0f, 0.0f };
		vertices[11].n = { 1.0f, 0.0f, 0.0f };

		// left face
		vertices[12].pos = { -side,  side,  side }; // 1
		vertices[13].pos = { -side,  side, -side }; // 0
		vertices[14].pos = { -side, -side, -side }; // 4
		vertices[15].pos = { -side, -side,  side }; // 5
		vertices[12].n = { -1.0f, 0.0f, 0.0f };
		vertices[13].n = { -1.0f, 0.0f, 0.0f };
		vertices[14].n = { -1.0f, 0.0f, 0.0f };
		vertices[15].n = { -1.0f, 0.0f, 0.0f };

		// top face
		vertices[16].pos = { -side,  side, -side }; // 0
		vertices[17].pos = { -side,  side,  side }; // 1
		vertices[18].pos = {  side,  side,  side }; // 2
		vertices[19].pos = {  side,  side, -side }; // 3
		vertices[16].n = { 0.0f, 1.0f, 0.0f };
		vertices[17].n = { 0.0f, 1.0f, 0.0f };
		vertices[18].n = { 0.0f, 1.0f, 0.0f };
		vertices[19].n = { 0.0f, 1.0f, 0.0f };

		// bottom face
		vertices[20].pos = { -side, -side, -side }; // 4
		vertices[21].pos = {  side, -side, -side }; // 7
		vertices[22].pos = {  side, -side,  side }; // 6
		vertices[23].pos = { -side, -side,  side }; // 5
		vertices[20].n = { 0.0f, -1.0f, 0.0f };
		vertices[21].n = { 0.0f, -1.0f, 0.0f };
		vertices[22].n = { 0.0f, -1.0f, 0.0f };
		vertices[23].n = { 0.0f, -1.0f, 0.0f };

		return {
			vertices, {
				0,1,2 , 0,2,3, // front face
				4,5,6 , 4,6,7, // back face
				8,9,10 , 8,10,11, // right face
				12,13,14 , 12,14,15, // left face
				16,17,18 , 16,18,19, // top face
				20,21,22 , 20,22,23 // bottom face
			}
		};
	}

private:
	std::vector<Vec3> vertices;
	const float side;
};