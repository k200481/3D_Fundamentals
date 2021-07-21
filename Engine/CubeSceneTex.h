#pragma once

#include "CubeScene.h"
#include "Surface.h"

class CubeSceneTex : public CubeScene
{
public:
	CubeSceneTex( std::wstring filename )
		:
		s( Surface::FromFile( filename ) )
	{}
	
	virtual void Render( Graphics& gfx ) const override
	{
		// get triangles and transform vertices from model space to world space
		auto triangles = c.GetTrianglesTex();
		const Mat3 m = Mat3::RotationX(theta_x) * Mat3::RotationY(theta_y) * Mat3::RotationZ(theta_z);
		for (auto& v : triangles.vertices)
		{
			v.pos *= m;
			v.pos += { 0.0f, 0.0f, zOffset };
		}

		// cull backfaces
		for (size_t i = 0, e = triangles.indices.size() / 3; i < e; i++)
		{
			const Vec3& v0 = triangles.vertices[triangles.indices[i * 3]].pos;
			const Vec3& v1 = triangles.vertices[triangles.indices[i * 3 + 1]].pos;
			const Vec3& v2 = triangles.vertices[triangles.indices[i * 3 + 2]].pos;

			triangles.cullingFlags[i] = (v1 - v0) % (v2 - v0) * v0 > 0.0f;
		}

		// transform vertices to screen/graphics space
		for (auto& v : triangles.vertices)
		{
			ct.Transform(v.pos);
		}

		// draw triangles
		for (size_t i = 0, e = triangles.indices.size() / 3; i < e; i++)
		{
			if (!triangles.cullingFlags[i]) {
				const TexVertex& v0 = triangles.vertices[triangles.indices[i * 3]];
				const TexVertex& v1 = triangles.vertices[triangles.indices[i * 3 + 1]];
				const TexVertex& v2 = triangles.vertices[triangles.indices[i * 3 + 2]];

				gfx.DrawTriangleTex(v0, v1, v2, s);
			}
		}
	}

private:
	Surface s;
};