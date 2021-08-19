#pragma once

#include "ChiliWin.h"
#include "Graphics.h"
#include "Triangle.h"
#include "IndexedTriangleList.h"
#include "NDCScreenTransformer.h"
#include "Mat.h"
#include <algorithm>

#include "ZBuffer.h"
#include <memory>

template <typename Effect>
class Pipeline
{
public:
	typedef typename Effect::Vertex Vertex;
	typedef typename Effect::VertexShader::Output VSOut;
	typedef typename Effect::GeometryShader::Output GSOut;
public:
	Pipeline(Graphics& gfx)
		:
		gfx(gfx),
		pZBuf( std::make_shared<ZBuffer>( Graphics::ScreenWidth, Graphics::ScreenHeight ) )
	{}
	Pipeline( Graphics& gfx, std::shared_ptr<ZBuffer> pZBuf_in )
		:
		gfx( gfx ),
		pZBuf( std::move(pZBuf_in) )
	{
		assert( pZBuf->GetWidth() == gfx.ScreenWidth && pZBuf->GetHeight() == gfx.ScreenHeight );
	}
	void BeginScene()
	{
		pZBuf->Clear();
	}
	void Draw(const IndexedTriangleList<Vertex>& triList)
	{
		ProcessVertices(triList.vertices, triList.indices);
	}
private:
	// transforms vertices to world space
	void ProcessVertices(const std::vector<Vertex>& vertices, const std::vector<size_t>& indices)
	{
		// create vertex vector for vs output
		std::vector<VSOut> verticesOut( vertices.size() );

		// perform vertex shader transformations
		std::transform( vertices.begin(), vertices.end(), verticesOut.begin(), effect.vs );

		// assemble triangles from stream of indices and vertices
		AssembleTriangles(verticesOut, indices);
	}
	// creates triangles from the vertices and indices
	// also culls backfaces
	void AssembleTriangles(const std::vector<VSOut>& vertices, const std::vector<size_t>& indices)
	{
		// assemble triangles in the stream and process
		for (size_t i = 0, end = indices.size() / 3;
			i < end; i++)
		{
			// determine triangle vertices via indexing
			const auto& v0 = vertices[indices[i * 3]];
			const auto& v1 = vertices[indices[i * 3 + 1]];
			const auto& v2 = vertices[indices[i * 3 + 2]];
			
			const auto viewPos = Vec4(0.0f, 0.0f, 0.0f, 1.0f) * effect.vs.GetProjection();
			// cull backfacing triangles with cross product (%) shenanigans
			if ((v1.pos - v0.pos) % (v2.pos - v0.pos) * (v0.pos - viewPos) <= 0.0f)
			{
				// process 3 vertices into a triangle
				ProcessTriangle( v0, v1, v2, i );
			}
		}
	}
	// creates a triangle object
	void ProcessTriangle(const VSOut& v0, const VSOut& v1, const VSOut& v2, size_t triangle_id)
	{
		ClipCullTriangle( effect.gs( v0, v1, v2, triangle_id ) );
	}
	// 
	void ClipCullTriangle(Triangle<GSOut>& t)
	{
		// cull tests
		if (t.v0.pos.x > t.v0.pos.w &&
			t.v1.pos.x > t.v1.pos.w &&
			t.v2.pos.x > t.v2.pos.w)
		{
			return;
		}
		if (t.v0.pos.x < -t.v0.pos.w &&
			t.v1.pos.x < -t.v1.pos.w &&
			t.v2.pos.x < -t.v2.pos.w)
		{
			return;
		}
		if (t.v0.pos.y > t.v0.pos.w &&
			t.v1.pos.y > t.v1.pos.w &&
			t.v2.pos.y > t.v2.pos.w)
		{
			return;
		}
		if (t.v0.pos.y < -t.v0.pos.w &&
			t.v1.pos.y < -t.v1.pos.w &&
			t.v2.pos.y < -t.v2.pos.w)
		{
			return;
		}
		if (t.v0.pos.z > t.v0.pos.w &&
			t.v1.pos.z > t.v1.pos.w &&
			t.v2.pos.z > t.v2.pos.w)
		{
			return;
		}
		if (t.v0.pos.z < 0.0f &&
			t.v1.pos.z < 0.0f &&
			t.v2.pos.z < 0.0f)
		{
			return;
		}

		// clipping routines
		// v0 is out behind the near plane
		const auto Clip1 = [this](const GSOut& v0, const GSOut& v1, const GSOut& v2) 
		{
			const float alpha1 = -v0.pos.z / (v1.pos.z - v0.pos.z);
			const float alpha2 = -v0.pos.z / (v2.pos.z - v0.pos.z);

			const auto v0a = interpolate(v0, v1, alpha1);
			const auto v0b = interpolate(v0, v2, alpha2);

			PostProcessTriangleVertices(Triangle<GSOut>{v0a, v1, v2});
			PostProcessTriangleVertices(Triangle<GSOut>{v0a, v2, v0b});
		};
		// v1 and v2 are behind the near plane
		const auto Clip2 = [this](const GSOut& v0, const GSOut& v1, const GSOut& v2)
		{
			const float alpha1 = -v1.pos.z / (v0.pos.z - v1.pos.z);
			const float alpha2 = -v2.pos.z / (v0.pos.z - v2.pos.z);

			const auto v1a = interpolate(v1, v0, alpha1);
			const auto v2a = interpolate(v2, v0, alpha2);

			PostProcessTriangleVertices(Triangle<GSOut>{v1a, v0, v2a});
		};

		// near plane clipping
		if (t.v0.pos.z < 0.0f)
		{
			if (t.v1.pos.z < 0.0f)
			{
				Clip2(t.v2, t.v0, t.v1);
			}
			else if (t.v2.pos.z < 0.0f)
			{
				Clip2(t.v1, t.v0, t.v2);
			}
			else
			{
				Clip1(t.v0, t.v1, t.v2);
			}
		}
		else if (t.v1.pos.z < 0.0f)
		{
			if (t.v2.pos.z < 0.0f)
			{
				Clip2(t.v0, t.v1, t.v2);
			}
			else
			{
				Clip1(t.v1, t.v0, t.v2);
			}
		}
		else if(t.v2.pos.z < 0.0f)
		{
			Clip1(t.v2, t.v1, t.v0);
		}
		else
		{
			PostProcessTriangleVertices(t);
		}
	}
	// transforms triangle's vertices to perspective/screen space
	void PostProcessTriangleVertices(Triangle<GSOut>& triangle)
	{
		ct.Transform( triangle.v0 );
		ct.Transform( triangle.v1 );
		ct.Transform( triangle.v2 );

		// draw the triangle
		DrawTriangle(triangle);
	}
	// identifies the type of triangle and calls either draw flat top or draw flat bottom
	// with the appropriate parameters
	void DrawTriangle(const Triangle<GSOut>& triangle)
	{
		// using pointers so we can swap (for sorting purposes)
		const GSOut* pv0 = &triangle.v0;
		const GSOut* pv1 = &triangle.v1;
		const GSOut* pv2 = &triangle.v2;

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
	void DrawFlatTopTriangle(const GSOut& it0, const GSOut& it1, const GSOut& it2)
	{
		// total y distance
		const float dy = it2.pos.y - it0.pos.y;
		// change in left edge for every change in y
		const auto di0 = ( it2 - it0 ) / dy;
		// change in right edge for every change in y
		const auto di1 = ( it2 - it1 ) / dy;

		// right edge, starts at v1 in this case
		const auto& itEdge1 = it1;

		DrawFlatTriangle( it0, it1, it2, di0, di1, itEdge1 );
	}
	// does the parts unique to flat bottom tirangles and calls draw flat triangle
	void DrawFlatBottomTriangle(const GSOut& it0, const GSOut& it1, const GSOut& it2)
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
	void DrawFlatTriangle(const GSOut& it0, const GSOut& it1, const GSOut& it2, const GSOut& dv0, const GSOut& dv1, GSOut itEdge1)
	{
		const int yStart = std::max( (int)std::ceil( it0.pos.y - 0.5f ), 0);
		const int yEnd = std::min( (int)std::ceil( it2.pos.y - 0.5f ), (int)Graphics::ScreenHeight - 1);

		// left edge, alwas v0
		auto itEdge0 = it0;

		// pre steps
		itEdge0 += dv0 * ( float(yStart) + 0.5f - it0.pos.y );
		itEdge1 += dv1 * ( float(yStart) + 0.5f - it0.pos.y );

		// loop
		for ( int y = yStart; y < yEnd; y++, itEdge0 += dv0, itEdge1 += dv1 )
		{
			const int xStart = std::max( (int)std::ceil(itEdge0.pos.x - 0.5f), 0);
			const int xEnd = std::min( (int)std::ceil(itEdge1.pos.x - 0.5f), (int)Graphics::ScreenWidth - 1);

			// the coordinate in perspective space to be drawn
			GSOut tc = itEdge0;
			// the rate at which this coordinate changes with every change in x
			GSOut dt = ( itEdge1 - itEdge0 ) / ( itEdge1.pos.x - itEdge0.pos.x );
			for ( int x = xStart; x < xEnd; x++, tc += dt )
			{
				// z actually stores zInv
				if ( pZBuf->TestAndSet( x, y, tc.pos.z ) )
				{
					const float w = 1.0f / tc.pos.w;
					// recover the coordinate in object space 
					// (technically world space since they are trandformed and rotated)
					// can't use tc here because it is being added into
					const auto v = tc * w;
					gfx.PutPixel(
						x, y,
						effect.ps( v )
					);
				}
			}
		}
	}

public:
	Effect effect;
private:
	Graphics& gfx;
	NDCScreenTransformer ct;
	std::shared_ptr<ZBuffer> pZBuf;
};