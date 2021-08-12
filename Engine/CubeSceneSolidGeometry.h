#pragma once

#include "CubeScene.h"
#include "SolidGeometryEffect.h"
#include "Pipeline.h"

#include "ColorBlendEffect.h"

class CubeSceneSolidGeometry : public CubeScene
{
	typedef Pipeline<SolidGeometryEffect> Pipeline;
	typedef Pipeline::Vertex Vertex;
public:
	CubeSceneSolidGeometry( Graphics& gfx )
		:
		pipeline( gfx ),
		triangles( Cube::GetPlain<Vertex>() )
	{
	}

	virtual void Render() override
	{
		pipeline.BeginScene();

		// get matrix to transform vertices from model space to world space
		const Mat3 m = Mat3::RotationX(theta_x) * Mat3::RotationY(theta_y) * Mat3::RotationZ(theta_z);

		pipeline.effect.vs.BindRotation(m);
		pipeline.effect.vs.BindTranslation({ 0.0f, 0.0f, zOffset });
		pipeline.Draw(triangles);
	}

private:
	Pipeline pipeline;
	const IndexedTriangleList<Vertex> triangles;
};