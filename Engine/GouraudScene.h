#pragma once

#include "CubeScene.h"
#include "Pipeline.h"
#include "GouraudEffect.h"

class GouraudScene : public CubeScene
{
	typedef Pipeline<GouraudEffect> Pipeline;
	typedef Pipeline::Vertex Vertex;
public:
	GouraudScene( Graphics& gfx, IndexedTriangleList<Vertex> itlist_in )
		:
		pipeline( gfx ),
		itlist( std::move(itlist_in) )
	{
		itlist.AdjustToTrueCenter();
		zOffset = itlist.GetRadius() * 1.6f;
	}

	virtual void Render() override
	{
		pipeline.BeginScene();

		const Mat3 rot = Mat3::RotationX( theta_x ) * Mat3::RotationY( theta_y ) * Mat3::RotationZ( theta_z );
		const Vec3 tl = { 0.0f, 0.0f, zOffset };

		pipeline.effect.vs.BindRotation(rot);
		pipeline.effect.vs.BindTranslation(tl);

		pipeline.effect.vs.SetLightDirection( { -1.0f, 0.0f, 0.5f } );

		pipeline.Draw(itlist);
	}

private:
	Pipeline pipeline;
	IndexedTriangleList<Vertex> itlist;
};