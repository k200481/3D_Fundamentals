#pragma once

#include "CubeScene.h"
#include "Pipeline.h"
#include "FlatGeometryEffect.h"

class FlatGeometryScene : public CubeScene
{
	typedef Pipeline<FlatGeometryEffect> Pipeline;
	typedef Pipeline::Vertex Vertex;
public:
	FlatGeometryScene( Graphics& gfx )
		:
		pipeline( gfx ),
		itlist( Cube::GetPlain<Vertex>() )
	{}

	virtual void Render() override
	{
		pipeline.BeginScene();

		const Mat3 rot = Mat3::RotationX( theta_x ) * Mat3::RotationY( theta_y ) * Mat3::RotationZ( theta_z );
		const Vec3 tl = { 0.0f, 0.0f, zOffset };

		pipeline.effect.vs.BindRotation( rot );
		pipeline.effect.vs.BindTranslation( tl );


		
		pipeline.Draw( itlist );
	}

private:
	Pipeline pipeline;
	IndexedTriangleList<Vertex> itlist;
};