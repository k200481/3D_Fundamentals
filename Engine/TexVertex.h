#pragma once

#include "Vec2.h"
#include "Vec3.h"

class TexVertex
{
public:
	TexVertex( const Vec3& pos, const Vec2& tc )
		:
		pos( pos ),
		tc( tc )
	{
	}
	TexVertex& InterpolateTo( const TexVertex& rhs, float alpha )
	{
		pos.InterpolateTo( rhs.pos, alpha );
		tc.InterpolateTo( rhs.tc, alpha );
		return *this;
	}
	TexVertex GetInterpotated( const TexVertex& rhs, float alpha ) const
	{
		return {
			pos.GetInterpolated( rhs.pos, alpha ),
			tc.GetInterpolated( rhs.tc, alpha )
		};
	}

	TexVertex operator+( const TexVertex& rhs ) const
	{
		return {
			pos + rhs.pos,
			tc + rhs.tc
		};
	}
	TexVertex& operator+=( const TexVertex& rhs )
	{
		return *this = *this + rhs;
	}
	TexVertex operator-( const TexVertex& rhs ) const
	{
		return {
			pos - rhs.pos,
			tc - rhs.tc
		};
	}
	TexVertex& operator-=( const TexVertex& rhs )
	{
		return *this = *this - rhs;
	}
	TexVertex operator*( float scale ) const
	{
		return {
			pos * scale,
			tc * scale
		};
	}
	TexVertex& operator*=( float scale )
	{
		return *this = *this * scale;
	}
	TexVertex operator/( float scale ) const
	{
		return {
			pos / scale,
			tc / scale
		};
	}
	TexVertex& operator/=( float scale )
	{
		return *this = *this / scale;
	}

public:
	Vec3 pos;
	Vec2 tc;
};

