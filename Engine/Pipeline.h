#pragma once

#include "ChiliWin.h"
#include "Graphics.h"
#include "Triangle.h"
#include "IndexedTriangleList.h"
#include "CoordinateTransformer.h"
#include "Mat3.h"
#include <algorithm>

template <typename Effect>
class Pipeline
{
public:
	typedef typename Effect::Vertex Vertex;
public:
	Pipeline(Graphics& gfx)
		:
		gfx(gfx)
	{}
	void Draw(const IndexedTriangleList<Vertex>& triList)
	{
		ProcessVertices(triList.vertices, triList.indices);
	}
	void BindRotation(const Mat3& rotation_in)
	{
		rotation = rotation_in;
	}
	void BindTranslation(const Vec3& translation_in)
	{
		translation = translation_in;
	}
private:
	// transforms vertices to world space
	void ProcessVertices(const std::vector<Vertex>& vertices, const std::vector<size_t>& indices)
	{
		// create vertex vector for vs output
		std::vector<Vertex> verticesOut;

		// transform vertices using matrix + vector
		for (const auto& v : vertices)
		{
			verticesOut.emplace_back( v.pos * rotation + translation, v );
		}

		// assemble triangles from stream of indices and vertices
		AssembleTriangles(verticesOut, indices);
	}
	// creates triangles from the vertices and indices
	// also culls backfaces
	void AssembleTriangles(const std::vector<Vertex>& vertices, const std::vector<size_t>& indices)
	{
		// assemble triangles in the stream and process
		for (size_t i = 0, end = indices.size() / 3;
			i < end; i++)
		{
			// determine triangle vertices via indexing
			const auto& v0 = vertices[indices[i * 3]];
			const auto& v1 = vertices[indices[i * 3 + 1]];
			const auto& v2 = vertices[indices[i * 3 + 2]];
			// cull backfacing triangles with cross product (%) shenanigans
			if ((v1.pos - v0.pos) % (v2.pos - v0.pos) * v0.pos <= 0.0f)
			{
				// process 3 vertices into a triangle
				ProcessTriangle(v0, v1, v2);
			}
		}
	}
	// creates a triangle object
	void ProcessTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
	{
		PostProcessTriangleVertices(Triangle<Vertex>{ v0, v1, v2 });
	}
	// transforms triangle's vertices to perspective/screen space
	void PostProcessTriangleVertices(Triangle<Vertex>& triangle)
	{
		ct.Transform( triangle.v0 );
		ct.Transform( triangle.v1 );
		ct.Transform( triangle.v2 );

		// draw the triangle
		DrawTriangle(triangle);
	}
	// identifies the type of triangle and calls either draw flat top or draw flat bottom
	// with the appropriate parameters
	void DrawTriangle(const Triangle<Vertex>& triangle)
	{
		// using pointers so we can swap (for sorting purposes)
		const Vertex* pv0 = &triangle.v0;
		const Vertex* pv1 = &triangle.v1;
		const Vertex* pv2 = &triangle.v2;

		// sorting vertices by y
		if (pv1->pos.y < pv0->pos.y) std::swap(pv0, pv1);
		if (pv2->pos.y < pv1->pos.y) std::swap(pv1, pv2);
		if (pv1->pos.y < pv0->pos.y) std::swap(pv0, pv1);

		if (pv0->pos.y == pv1->pos.y) // natural flat top
		{
			// sorting top vertices by x
			if (pv1->pos.x < pv0->pos.x) std::swap(pv0, pv1);

			DrawFlatTopTriangle(*pv0, *pv1, *pv2);
		}
		else if (pv1->pos.y == pv2->pos.y) // natural flat bottom
		{
			// sorting bottom vertices by x
			if (pv2->pos.x < pv1->pos.x) std::swap(pv1, pv2);

			DrawFlatBottomTriangle(*pv0, *pv1, *pv2);
		}
		else // general triangle
		{
			// find splitting vertex interpolant
			const float alphaSplit =
				(pv1->pos.y - pv0->pos.y) /
				(pv2->pos.y - pv0->pos.y);
			const auto vi = interpolate(*pv0, *pv2, alphaSplit);

			if (pv1->pos.x < vi.pos.x) // major right
			{
				DrawFlatBottomTriangle(*pv0, *pv1, vi);
				DrawFlatTopTriangle(*pv1, vi, *pv2);
			}
			else // major left
			{
				DrawFlatBottomTriangle(*pv0, vi, *pv1);
				DrawFlatTopTriangle(vi, *pv1, *pv2);
			}
		}
	}
	// does the parts unique to flat top triangles and calls draw flat triangle 
	void DrawFlatTopTriangle(const Vertex& it0, const Vertex& it1, const Vertex& it2)
	{
		// total y distance
		const float dy = it2.pos.y - it0.pos.y;
		// change in left edge for every change in y
		const auto di0 = ( it2 - it0 ) / dy;
		// change in right edge for every change in y
		const auto di1 = ( it2 - it1 ) / dy;

		// right edge, starts at v1 in this case
		const auto itEdge1 = it1;

		DrawFlatTriangle( it0, it1, it2, di0, di1, itEdge1 );
	}
	// does the parts unique to flat bottom tirangles and calls draw flat triangle
	void DrawFlatBottomTriangle(const Vertex& it0, const Vertex& it1, const Vertex& it2)
	{
		// total y distance
		const float dy = it2.pos.y - it0.pos.y;
		// change in left edge for every change in y
		const auto di0 = ( it1 - it0 ) / dy;
		// change in rught edge for every change in y
		const auto di1 = ( it2 - it0 ) / dy;

		// right edge, starts at v0 in this case
		const auto itEdge1 = it0;

		DrawFlatTriangle( it0, it1, it2, di0, di1, itEdge1 );
	}
	// draws a flat triangle, handles the parts in common in both flat top and flat bottom triangles
	void DrawFlatTriangle(const Vertex& it0, const Vertex& it1, const Vertex& it2, const Vertex& dv0, const Vertex& dv1, Vertex itEdge1)
	{
		const int yStart = (int)std::ceil( it0.pos.y - 0.5f );
		const int yEnd = (int)std::ceil( it2.pos.y - 0.5f );

		// left edge, alwas v0
		auto itEdge0 = it0;

		// pre steps
		itEdge0 += dv0 * ( float(yStart) + 0.5f - it0.pos.y );
		itEdge1 += dv1 * ( float(yStart) + 0.5f - it0.pos.y );

		// loop
		for ( int y = yStart; y < yEnd; y++, itEdge0 += dv0, itEdge1 += dv1 )
		{
			const int xStart = (int)std::ceil(itEdge0.pos.x - 0.5f);
			const int xEnd = (int)std::ceil(itEdge1.pos.x - 0.5f);

			// the coordinate in perspective space to be drawn
			Vertex tc = itEdge0;
			// the rate at which this coordinate changes with every change in x
			Vertex dt = ( itEdge1 - itEdge0 ) / ( itEdge1.pos.x - itEdge0.pos.x );
			for ( int x = xStart; x < xEnd; x++, tc += dt )
			{
				// z actually stores zInv
				const float z = 1.0f / tc.pos.z;
				// recover the coordinate in object space 
				// (technically world space since they are trandformed and rotated)
				// can't use tc here because it is being added into
				const auto v = tc * z;
				gfx.PutPixel(
					x, y,
					effect.ps( v )
				);
			}
		}
	}

public:
	Effect effect;
private:
	Graphics& gfx;
	CoordinateTransformer ct;
	
	Mat3 rotation = Mat3::Identity();
	Vec3 translation = { 0.0f,0.0f,0.0f };
};