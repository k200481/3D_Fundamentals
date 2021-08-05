#pragma once

#include "CubeScene.h"
#include "Pipeline.h"

class CubeSceneTex : public CubeScene
{
public:
	CubeSceneTex( Graphics& gfx, std::wstring filename )
		:
		pipeline( gfx )
	{
		pipeline.BindTexture( filename );
	}
	
	virtual void Render( Graphics& gfx ) override
	{
		// get triangles and transform vertices from model space to world space
		auto triangles = c.GetTrianglesTexUnfolded( 2.0f );
		const Mat3 m = Mat3::RotationX(theta_x) * Mat3::RotationY(theta_y) * Mat3::RotationZ(theta_z);

		pipeline.BindRotation( m );
		pipeline.BindTranslation({ 0.0f, 0.0f, zOffset });
		pipeline.Draw( c.GetTrianglesTex() );
	}

private:
	Pipeline pipeline;
};