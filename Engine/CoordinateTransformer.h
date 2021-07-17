#pragma once

#include "Graphics.h"

class CoordinateTransformer
{
public:
	CoordinateTransformer()
		:
		xFactor( Graphics::ScreenWidth / 2 - 1 ),
		yFactor( Graphics::ScreenHeight / 2 - 1 )
	{
	}

	Vec2& Transform( Vec2& v ) const
	{
		v.x = (  v.x + 1.0f ) * xFactor;
		v.y = ( -v.y + 1.0f ) * yFactor;
		return v;
	}
	Vec2 GetTransformed( const Vec2& v ) const
	{
		return Transform( Vec2(v) );
	}

private:
	const float xFactor;
	const float yFactor;
};