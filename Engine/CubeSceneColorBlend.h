#pragma once

#include "CubeScene.h"
#include "Pipeline.h"
#include "ColorBlendEffect.h"

class CubeSceneColorBlend : public CubeScene
{
	typedef Pipeline<ColorBlendEffect> Pipeline;
	typedef ColorBlendEffect::Vertex Vertex;
public:
	CubeSceneColorBlend( Graphics& gfx )
		:
		pipeline( gfx )
	{
	}
	
	virtual void Render() override
	{
		IndexedTriangleList<Vertex> triangles = Cube::GetPlain<Vertex>();

		triangles.vertices[0].color = Vec3( Colors::Red );
		triangles.vertices[1].color = Vec3( Colors::Blue );
		triangles.vertices[2].color = Vec3( Colors::Green );
		triangles.vertices[3].color = Vec3( Colors::Black );
		triangles.vertices[4].color = Vec3( Colors::Yellow );
		triangles.vertices[5].color = Vec3( Colors::White );
		triangles.vertices[6].color = Vec3( Colors::Cyan );
		triangles.vertices[7].color = Vec3( Colors::Magenta );

		// get matrix to transform vertices from model space to world space
		const Mat3 m = Mat3::RotationX(theta_x) * Mat3::RotationY(theta_y) * Mat3::RotationZ(theta_z);

		pipeline.BindRotation( m );
		pipeline.BindTranslation({ 0.0f, 0.0f, zOffset });
		pipeline.Draw( triangles );
	}

private:
	Pipeline pipeline;
};