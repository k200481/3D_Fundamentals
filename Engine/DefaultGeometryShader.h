#pragma once

#include "Triangle.h"

template <class Vertex>
class DefaultGeometryShader
{
public:
	// output for pipeline
	// returns the same vertex that was taken in with no changes
	typedef Vertex Output;

	Triangle<Output> operator()( const Vertex& v0, const Vertex& v1, const Vertex& v2, size_t triangle_id )
	{
		return { v0, v1, v2 };
	}
};