#pragma once

#include "Graphics.h"
#include "Vec3.h"

class CoordinateTransformer
{
public:
	CoordinateTransformer()
		:
		xFactor( Graphics::ScreenWidth / 2 - 1 ),
		yFactor( Graphics::ScreenHeight / 2 - 1 )
	{
	}

	template <typename Vertex>
	Vertex& Transform( Vertex& v ) const
	{
		const float zInv = 1.0f / v.pos.z;

		v *= zInv;
		v.pos.x = ( v.pos.x + 1.0f ) * xFactor;
		v.pos.y = ( -v.pos.y + 1.0f ) * yFactor;

		v.pos.z = zInv;

		return v;
	}
	template <typename Vertex>
	Vertex GetTransformed( const Vertex& v ) const
	{
		return Transform( Vertex(v) );
	}

private:
	const float xFactor;
	const float yFactor;
};