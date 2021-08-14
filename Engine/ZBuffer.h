#pragma once

#include <memory>
#include <cassert>
#include <limits>

class ZBuffer
{
public:
	ZBuffer( size_t width, size_t height )
		:
		width( width ),
		height( height ),
		pBuf( std::make_unique<float[]>( width * height ) )
	{
	}

	void Clear()
	{
		const size_t nCells = width * height;
		for ( size_t i = 0; i < nCells; i++ )
		{
			pBuf[i] = std::numeric_limits<float>::infinity();
		}
	}

	float& At( int x, int y )
	{
		assert( x >= 0 );
		assert( y >= 0 );
		assert( x <= width );
		assert( y <= height );
		return pBuf[ width * y + x ];
	}
	const float& At( int x, int y ) const
	{
		assert( x >= 0 );
		assert( y >= 0 );
		assert( x <= width );
		assert( y <= height );
		return pBuf[ width * y + x ];
	}

	bool TestAndSet( int x, int y, float z )
	{
		float& c = At( x, y );
		if ( c > z )
		{
			c = z;
			return true;
		}
		return false;
	}

	size_t GetWidth() const
	{
		return width;
	}
	size_t GetHeight() const
	{
		return height;
	}

private:
	std::unique_ptr<float[]> pBuf;
	const size_t width;
	const size_t height;
};