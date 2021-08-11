#pragma once

#include "CubeScene.h"
#include "Pipeline.h"
#include "TextureEffect.h"

class CubeSceneTex : public CubeScene
{
	typedef Pipeline<TextureEffect> Pipeline;
	typedef TextureEffect::Vertex Vertex;
public:
	CubeSceneTex( Graphics& gfx, std::wstring filename )
		:
		pipeline( gfx )
	{
		pipeline.effect.ps.BindTexture( filename );
	}
	
	virtual void Render() override
	{
		pipeline.BeginScene();
		// get matrix to transform vertices from model space to world space
		const Mat3 m = Mat3::RotationX(theta_x) * Mat3::RotationY(theta_y) * Mat3::RotationZ(theta_z);

		pipeline.effect.vs.BindRotation( m );
		pipeline.effect.vs.BindTranslation({ 0.0f, 0.0f, zOffset });
		pipeline.Draw( c.GetTrianglesTex<Vertex>() );
	}

private:
	Pipeline pipeline;
};