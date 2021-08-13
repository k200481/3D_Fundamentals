#pragma once

#include "CubeScene.h"
#include "FlatVertexEffect.h"
#include "Pipeline.h"

class FlatVertexScene : public CubeScene
{
	typedef Pipeline<FlatVertexEffect> Pipeline;
	typedef Pipeline::Vertex Vertex;
public:
	FlatVertexScene( Graphics& gfx )
		:
		pipeline( gfx ),
		itlist( Cube::GetPlainIndependentFacesNormals<Vertex>() )
	{
	}

	virtual void Render() override
	{
		pipeline.BeginScene();

		const Mat3 rot = Mat3::RotationX( theta_x ) * Mat3::RotationY( theta_y ) * Mat3::RotationZ( theta_z );
		const Vec3 tl = { 0.0f, 0.0f, zOffset };

		pipeline.effect.vs.BindRotation( rot );
		pipeline.effect.vs.BindTranslation( tl );

		pipeline.effect.vs.SetLightDirection( { 0.0f, -1.0f, 1.0f } );
		pipeline.effect.vs.SetLightColor( Color(Vec3( 0.5f, 0.5f, 0.5f ) * 255.0f) );
		pipeline.effect.vs.SetAmbientLight( Color(Vec3( 0.2f, 0.1f, 0.0f ) * 255.0f) );
		pipeline.effect.vs.SetMaterialColor( Color( Vec3( 0.2f, 0.2f, 1.0f ) * 255.0f ) );
		
		pipeline.Draw( itlist );
	}

private:
	IndexedTriangleList<Vertex> itlist;
	Pipeline pipeline;
};