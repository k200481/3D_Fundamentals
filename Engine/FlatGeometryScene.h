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
		itlist( IndexedTriangleList<Vertex>::LoadFromFile("bunny.obj") )
	{
		itlist.AdjustToTrueCenter();
		zOffset = itlist.GetRadius() * 1.6f;
	}

	virtual void Render() override
	{
		pipeline.BeginScene();

		const Mat3 rot = Mat3::RotationX( theta_x ) * Mat3::RotationY( theta_y ) * Mat3::RotationZ( theta_z );
		const Vec3 tl = { 0.0f, 0.0f, zOffset };

		pipeline.effect.vs.BindRotation( rot );
		pipeline.effect.vs.BindTranslation( tl );
		pipeline.effect.gs.SetLightDirection( { 0.0f, -1.0f, -1.0f } );
		pipeline.effect.gs.SetMaterialColor( Colors::Yellow );

		const Vec3 light = Vec3{ 1.0f, 0.5f, 0.5f } * 255.0f;

		pipeline.effect.gs.SetLightColor( Color( light ) );
		pipeline.effect.gs.SetAmbientLight( Color( light * 0.1 ) );
		
		pipeline.Draw( itlist );
	}

private:
	Pipeline pipeline;
	IndexedTriangleList<Vertex> itlist;
};