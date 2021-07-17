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

	Vec3& Transform( Vec3& v ) const
	{
		const float zInv = 1.0f / v.z;
		v.x = ( (  v.x * zInv  ) + 1.0f ) * xFactor;
		v.y = ( ( -v.y * zInv ) + 1.0f ) * yFactor;
		return v;
	}
	Vec3 GetTransformed( const Vec3& v ) const
	{
		return Transform( Vec3(v) );
	}

private:
	const float xFactor;
	const float yFactor;
};