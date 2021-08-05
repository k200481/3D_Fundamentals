#pragma once

#include "ChiliWin.h"
#include "Graphics.h"
#include "Triangle.h"
#include "IndexedTriangleList.h"
#include "CoordinateTransformer.h"
#include "Mat3.h"
#include <algorithm>


class Pipeline
{
public:
	// vertex type used for geometry and throughout pipeline
	class Vertex
	{
	public:
		Vertex() = default;
		Vertex(const Vec3& pos)
			:
			pos(pos)
		{}
		// this enables template functions clone a vertex
		// while changing the pos only
		Vertex(const Vec3& pos, const Vertex& src)
			:
			t(src.t),
			pos(pos)
		{}
		Vertex(const Vec3& pos, const Vec2& t)
			:
			t(t),
			pos(pos)
		{}
		Vertex& operator+=(const Vertex& rhs)
		{
			pos += rhs.pos;
			t += rhs.t;
			return *this;
		}
		Vertex operator+(const Vertex& rhs) const
		{
			return Vertex(*this) += rhs;
		}
		Vertex& operator-=(const Vertex& rhs)
		{
			pos -= rhs.pos;
			t -= rhs.t;
			return *this;
		}
		Vertex operator-(const Vertex& rhs) const
		{
			return Vertex(*this) -= rhs;
		}
		Vertex& operator*=(float rhs)
		{
			pos *= rhs;
			t *= rhs;
			return *this;
		}
		Vertex operator*(float rhs) const
		{
			return Vertex(*this) *= rhs;
		}
		Vertex& operator/=(float rhs)
		{
			pos /= rhs;
			t /= rhs;
			return *this;
		}
		Vertex operator/(float rhs) const
		{
			return Vertex(*this) /= rhs;
		}
	public:
		Vec3 pos;
		Vec2 t;
	};
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
	void BindTexture(const std::wstring& filename)
	{
		pTex = std::make_unique<Surface>(Surface::FromFile(filename));
	}
private:
	void ProcessVertices(const std::vector<Vertex>& vertices, const std::vector<size_t>& indices)
	{
		// create vertex vector for vs output
		std::vector<Vertex> verticesOut;

		// transform vertices using matrix + vector
		for (const auto& v : vertices)
		{
			verticesOut.emplace_back(v.pos * rotation + translation, v.t);
		}

		// assemble triangles from stream of indices and vertices
		AssembleTriangles(verticesOut, indices);
	}
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
	void ProcessTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
	{
		PostProcessTriangleVertices(Triangle<Vertex>{ v0, v1, v2 });
	}
	void PostProcessTriangleVertices(Triangle<Vertex>& triangle)
	{
		ct.Transform(triangle.v0.pos);
		ct.Transform(triangle.v1.pos);
		ct.Transform(triangle.v2.pos);

		// draw the triangle
		DrawTriangle(triangle);
	}

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
	void DrawFlatTopTriangle(const Vertex& it0, const Vertex& it1, const Vertex& it2)
	{
		const float dy = it2.pos.y - it0.pos.y;
		const auto di0 = ( it2 - it0 ) / dy;
		const auto di1 = ( it2 - it1 ) / dy;

		const auto itEdge1 = it1;

		DrawFlatTriangle( it0, it1, it2, di0, di1, itEdge1 );
	}

	void DrawFlatBottomTriangle(const Vertex& it0, const Vertex& it1, const Vertex& it2)
	{
		const float dy = it2.pos.y - it0.pos.y;
		const auto di0 = ( it1 - it0 ) / dy;
		const auto di1 = ( it2 - it0 ) / dy;

		const auto itEdge1 = it0;

		DrawFlatTriangle( it0, it1, it2, di0, di1, itEdge1 );
	}
	
	void DrawFlatTriangle(const Vertex& it0, const Vertex& it1, const Vertex& it2, const Vertex& dv0, const Vertex& dv1, Vertex itEdge1)
	{
		const int yStart = (int)std::ceil( it0.pos.y - 0.5f );
		const int yEnd = (int)std::ceil( it2.pos.y - 0.5f );

		auto itEdge0 = it0;

		itEdge0 += dv0 * ( float(yStart) + 0.5f - it0.pos.y );
		itEdge1 += dv1 * ( float(yStart) + 0.5f - it0.pos.y );

		// constants
		const float texWidth = (float)pTex->GetWidth();
		const float texHeight = (float)pTex->GetHeight();
		const float tex_xClamp = texWidth - 1.0f;
		const float tex_yClamp = texHeight - 1.0f;

		// loop
		for ( int y = yStart; y < yEnd; y++, itEdge0 += dv0, itEdge1 += dv1 )
		{
			const int xStart = (int)std::ceil(itEdge0.pos.x - 0.5f);
			const int xEnd = (int)std::ceil(itEdge1.pos.x - 0.5f);

			Vertex tc = itEdge0;
			Vertex dt = ( itEdge1 - itEdge0 ) / ( itEdge1.pos.x - itEdge0.pos.x );
			for ( int x = xStart; x < xEnd; x++, tc += dt )
			{
				gfx.PutPixel(
					x, y,
					pTex->GetPixel( (int)std::min( tc.t.x * texWidth, tex_xClamp ), 
									(int)std::min( tc.t.y * texHeight, tex_yClamp )
					)
				);
			}
		}
	}
private:
	Graphics& gfx;
	CoordinateTransformer ct;
	
	Mat3 rotation;
	Vec3 translation;
	std::unique_ptr<Surface> pTex;
};