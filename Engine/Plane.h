#pragma once

#include "IndexedTriangleList.h"
#include <array>

class Plane
{
public:

	template <class Vertex>
	static IndexedTriangleList<Vertex> GetPlain( int divisions = 7, float size = 1.0f )
	{
		// number of vertices on each side
		const int nVerticesSide = divisions + 1;
		const float side = size / 2.0f;
		const float divSide = size / divisions;

		std::vector<Vertex> vertices( sq(nVerticesSide) );
		// starting from here because we can just add x and y values to get the next vertex
		const Vec3 bottomLeft = { -side, -side, 0.0f };
		// generate all the vertices
		for ( size_t y = 0, i = 0; y < nVerticesSide; y++ )
		{
			for ( size_t x = 0; x < nVerticesSide; x++, i++ )
			{
				vertices[i] = bottomLeft + Vec3( x * divSide, y * divSide, 0.0f );
			}
		}

		std::vector<size_t> indices( sq(divisions) * 6 );

		// converts vertices to indices
		auto vxy2i = [nVerticesSide]( size_t x, size_t y ) 
		{
			return y * nVerticesSide + x;
		};

		for ( size_t y = 0, i = 0; y < divisions; y++ )
		{

			for ( size_t x = 0; x < divisions; x++, i += 6 )
			{
				std::array<size_t, 4> indexArray = { 
					vxy2i( x,y ), vxy2i( x+1,y ), vxy2i( x,y+1 ), vxy2i( x+1,y+1 )
				};

				indices[i]   = indexArray[0];
				indices[i+1] = indexArray[2];
				indices[i+2] = indexArray[1];
				indices[i+3] = indexArray[1];
				indices[i+4] = indexArray[2];
				indices[i+5] = indexArray[3];
			}
		}
		return { std::move( vertices ), std::move( indices ) };
	}

	template <class Vertex>
	static IndexedTriangleList<Vertex> GetNormals( int divisions = 7, float size = 1.0f )
	{
		auto tl = GetPlain<Vertex>( divisions, size );
		for ( auto& v : tl.vertices )
		{
			v.n = { 0.0f, 0.0f, -1.0f };
		}
		return std::move(tl);
	}

	template <class Vertex>
	static IndexedTriangleList<Vertex> GetSkinned( int divisions = 7, float size = 1.0f )
	{
		auto triangles = GetPlain<Vertex>( divisions, size );
		// number of vertices on each side
		const int nVerticesSide = divisions + 1;
		// division size in the texture
		const float tDivSize = 1.0f / divisions;

		const Vec2 bottomLeft = { 0.0f, 1.0f };

		for ( size_t y = 0, i = 0; y < nVerticesSide; y++ )
		{
			for ( size_t x = 0; x < nVerticesSide; x++, i++ )
			{
				triangles.vertices[i].t = bottomLeft + Vec2( float(x) * tDivSize, - float(y) * tDivSize );
			}
		}

		return std::move( triangles );
	}
};