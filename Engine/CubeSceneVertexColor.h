#pragma once

#include "CubeScene.h"
#include "Pipeline.h"
#include "VertexColorEffect.h"

class CubeSceneVertexColor : public CubeScene
{
	typedef Pipeline<VertexColorEffect> Pipeline;
	typedef VertexColorEffect::Vertex Vertex;
public:
	CubeSceneVertexColor( Graphics& gfx )
		:
		pipeline( gfx ),
		triangles( Cube::GetPlain<Vertex>() )
	{
	}
	virtual void Render() override
	{
		pipeline.BeginScene();

		const Mat3 m = Mat3::RotationX( theta_x ) * Mat3::RotationY( theta_y ) * Mat3::RotationZ( theta_z );
		const Vec3 translation = { 0.0f, 0.0f, zOffset };

		pipeline.effect.vs.BindRotation( m );
		pipeline.effect.vs.BindTranslation( translation );

		pipeline.Draw( triangles );
	}

private:
	const IndexedTriangleList<Vertex> triangles;
	Pipeline pipeline;
};