#pragma once

#include <vector>
#include "Vec3.h"
#include "IndexedLineList.h"
#include "IndexedTriangleList.h"

class Cube
{
public:
	Cube( float size )
	{
		size /= 2.0f;

		vertices.emplace_back( -size,  size, -size );
		vertices.emplace_back( -size,  size,  size );
		vertices.emplace_back(  size,  size,  size );
		vertices.emplace_back(  size,  size, -size );
		vertices.emplace_back( -size, -size, -size );
		vertices.emplace_back( -size, -size,  size );
		vertices.emplace_back(  size, -size,  size );
		vertices.emplace_back(  size, -size, -size );
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
	IndexedTriangleList GetTriangles() const
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

private:
	std::vector<Vec3> vertices;
};