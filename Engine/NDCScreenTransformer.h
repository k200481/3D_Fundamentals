#pragma once

#include "Graphics.h"
#include "Vec4.h"

class NDCScreenTransformer
{
public:
	NDCScreenTransformer()
		:
		xFactor( Graphics::ScreenWidth / 2 - 1 ),
		yFactor( Graphics::ScreenHeight / 2 - 1 )
	{
	}

	template <typename Vertex>
	Vertex& Transform( Vertex& v ) const
	{
		const float wInv = 1.0f / v.pos.w;

		v *= wInv;

		v.pos.x = ( v.pos.x + 1.0f ) * xFactor;
		v.pos.y = ( -v.pos.y + 1.0f ) * yFactor;

		v.pos.w = wInv;

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