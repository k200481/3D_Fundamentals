#pragma once

#include "CubeScene.h"

class CubeSceneSolid : public CubeScene
{
public:
	CubeSceneSolid() = default;
	virtual  ~CubeSceneSolid() = default;

	virtual void Render(Graphics& gfx) const override
	{
		// the colors of the sides of the cube
		std::vector<Color> colors = {
			Colors::Yellow,
			Colors::Red,
			Colors::Blue,
			Colors::White,
			Colors::Green,
			Colors::Cyan
		};

		// get triangles and transform vertices from model space to world space
		auto triangles = c.GetTriangles();
		const Mat3 m = Mat3::RotationX(theta_x) * Mat3::RotationY(theta_y) * Mat3::RotationZ(theta_z);
		for (auto& v : triangles.vertices)
		{
			v *= m;
			v += { 0.0f, 0.0f, zOffset };
		}

		// cull backfaces
		for (size_t i = 0, e = triangles.indices.size() / 3; i < e; i++)
		{
			const Vec3& v0 = triangles.vertices[triangles.indices[i * 3]];
			const Vec3& v1 = triangles.vertices[triangles.indices[i * 3 + 1]];
			const Vec3& v2 = triangles.vertices[triangles.indices[i * 3 + 2]];

			triangles.cullingFlags[i] = (v1 - v0) % (v2 - v0) * v0 > 0.0f;
		}

		// transform vertices to screen/graphics space
		for (auto& v : triangles.vertices)
		{
			ct.Transform(v);
		}

		// draw triangles
		for (size_t i = 0, e = triangles.indices.size() / 3; i < e; i++)
		{
			if (!triangles.cullingFlags[i]) {
				const Vec3& v0 = triangles.vertices[triangles.indices[i * 3]];
				const Vec3& v1 = triangles.vertices[triangles.indices[i * 3 + 1]];
				const Vec3& v2 = triangles.vertices[triangles.indices[i * 3 + 2]];

				gfx.DrawTriangle(v0, v1, v2, colors[i / 2]);
			}
		}
	}
};