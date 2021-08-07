#pragma once

#include "CubeScene.h"
#include "Pipeline.h"
#include "SolidColorEffect.h"

class CubeSceneSolid : public CubeScene
{
	typedef Pipeline<SolidColorEffect> Pipeline;
	typedef SolidColorEffect::Vertex Vertex;
public:
	CubeSceneSolid( Graphics& gfx )
		:
		pipeline( gfx )
	{
	}
	virtual ~CubeSceneSolid() = default;

	virtual void Render() override
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
		auto triangles = Cube::GetPlainIndependentFaces<Vertex>();
		const Mat3 m = Mat3::RotationX(theta_x) * Mat3::RotationY(theta_y) * Mat3::RotationZ(theta_z);

		for ( size_t i = 0, e = triangles.vertices.size(); i < e; i++ )
		{
			triangles.vertices[i].color = colors[i / 4];
		}

		pipeline.BindRotation( m );
		pipeline.BindTranslation( { 0.0f, 0.0f, zOffset } );
		
		pipeline.Draw( triangles );
	}

private:
	Pipeline pipeline;
};