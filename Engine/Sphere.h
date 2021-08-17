#pragma once

#include "IndexedTriangleList.h"
#include "Mat.h"
#include "ChiliMath.h"

class Sphere
{
public:
	template <class Vertex>
	static IndexedTriangleList<Vertex> GetPlain( float radius, size_t latDiv, size_t longDiv )
	{
		IndexedTriangleList<Vertex> itlist;

		const float latAngle = PI / latDiv;
		const float longAngle = 2.0f * PI / latDiv;
		// pointing towards +ve z (north pole)
		const Vec3 base = { 0.0f, 0.0f, radius };

		itlist.vertices.reserve( latDiv * longDiv + 2 );
		// loop through all the lattitudes
		// and generate longitudinal bands
		for ( size_t i = 1; i < latDiv; i++ )
		{
			// the current latitudinal rotaion
			// obtained by rotating the base (north pole) around the x axis by the div number * angle per div
			const auto latRot = Mat3::RotationX( latAngle * float(i) );
			// loop through all the longitudes on the current latitude
			for ( size_t j = 0; j < longDiv; j++ )
			{
				// the current longitudinal rotation
				const auto longRot = Mat3::RotationZ( longAngle * float(j) );
				// def construct
				itlist.vertices.emplace_back();
				// generate the vertex by rotating base
				itlist.vertices.back().pos = base * latRot * longRot;
			}
		}

		// insert poles
		// top index
		const auto iNorth = itlist.vertices.size();
		itlist.vertices.emplace_back();
		itlist.vertices.back().pos = base;
		// bottom index
		const auto iSouth = itlist.vertices.size();
		itlist.vertices.emplace_back();
		itlist.vertices.back().pos = -base;

		
		// generate indices

		// lambda to get indices
		const auto GetIndex = [longDiv]( size_t lat, size_t lon )
		{
			return lat * longDiv + lon;
		};
		// loop through latitudes
		for ( size_t i = 0; i < latDiv - 2u; i++ )
		{
			// loop through longitudes except last one for each band
			for ( size_t j = 0; j < longDiv - 1u; j++ )
			{
				itlist.indices.push_back( GetIndex( i, j ) );
				itlist.indices.push_back( GetIndex( i + 1, j ) );
				itlist.indices.push_back( GetIndex( i, j + 1 ) );
				itlist.indices.push_back( GetIndex( i, j + 1 ) );
				itlist.indices.push_back( GetIndex( i + 1, j ) );
				itlist.indices.push_back( GetIndex( i + 1, j + 1 ) );
			}
			// wrapping
			itlist.indices.push_back( GetIndex( i, 0 ) );
			itlist.indices.push_back( GetIndex( i, longDiv - 1 ) );
			itlist.indices.push_back( GetIndex( i + 1, 0 ) );
			itlist.indices.push_back( GetIndex( i + 1, 0 ) );
			itlist.indices.push_back( GetIndex( i, longDiv - 1 ) );
			itlist.indices.push_back( GetIndex( i + 1, longDiv - 1 ) );
		}

		// generate indices for top and bottom bands

		// north
		for ( size_t i = 0; i < longDiv - 1u; i++ )
		{
			itlist.indices.push_back( iNorth );
			itlist.indices.push_back( GetIndex( 0, i ) );
			itlist.indices.push_back( GetIndex( 0, i + 1 ) );
		}
		itlist.indices.push_back( iNorth );
		itlist.indices.push_back( GetIndex( 0, longDiv - 1u ) );
		itlist.indices.push_back( GetIndex( 0, 0 ) );

		// south
		for ( size_t i = 0; i < longDiv - 1u; i++ )
		{
			itlist.indices.push_back( iSouth );
			itlist.indices.push_back( GetIndex( latDiv - 2u, i + 1 ) );
			itlist.indices.push_back( GetIndex( latDiv - 2u, i ) );
		}
		itlist.indices.push_back( iSouth );
		itlist.indices.push_back( GetIndex( latDiv - 2u, 0 ) );
		itlist.indices.push_back( GetIndex( latDiv - 2u, longDiv - 1u ) );

		return itlist;
	}
	template <class Vertex>
	static IndexedTriangleList<Vertex> GetNormals( float radius, size_t latDive, size_t longDiv )
	{
		auto itlist = GetPlain<Vertex>( radius, latDive, longDiv );

		for ( auto& v : itlist.vertices )
		{
			v.n = v.pos.GetNormalized();
		}
		return std::move( itlist );
	}
};