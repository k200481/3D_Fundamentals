#pragma once

#include "Scene.h"
#include "Pipeline.h"
#include "WaveEffect.h"
#include "Plane.h"

class PlaneWaveScene : public Scene
{
	typedef Pipeline<WaveEffect> Pipeline;
	typedef WaveEffect::Vertex Vertex;
public:
	PlaneWaveScene( Graphics& gfx )
		:
		pipeline( gfx ),
		itList( Plane::GetSkinned<Vertex>( 20 ) )
	{
		pipeline.effect.ps.BindTexture( L"abby.jpg" );
	}

	virtual void UpdateScene( Mouse& mouse, const Keyboard& kbd, float dt ) override
	{
		const float dTheta = PI / 2;

		if (kbd.KeyIsPressed(VK_UP))
		{
			zOffset += 0.1f;
		}
		else if (kbd.KeyIsPressed(VK_DOWN))
		{
			zOffset -= 0.1f;
		}

		if (kbd.KeyIsPressed('Q'))
		{
			theta_z += dt * dTheta;
		}
		else if (kbd.KeyIsPressed('E'))
		{
			theta_z -= dt * dTheta;
		}

		if (kbd.KeyIsPressed('W'))
		{
			theta_x += dt * dTheta;
		}
		else if (kbd.KeyIsPressed('S'))
		{
			theta_x -= dt * dTheta;
		}

		if (kbd.KeyIsPressed('A'))
		{
			theta_y += dt * dTheta;
		}
		else if (kbd.KeyIsPressed('D'))
		{
			theta_y -= dt * dTheta;
		}
		theta_x = wrap_angle(theta_x);
		theta_y = wrap_angle(theta_y);
		theta_z = wrap_angle(theta_z);

		seconds += dt;
		pipeline.effect.vs.SetTime( seconds );
	}
	virtual void Render() override
	{
		pipeline.BeginScene();

		Mat3 m = Mat3::RotationX( theta_x ) * Mat3::RotationY( theta_y ) * Mat3::RotationZ( theta_z );
		Vec3 v = { 0.0f, 0.0f, zOffset };

		pipeline.effect.vs.BindRotation( m );
		pipeline.effect.vs.BindTranslation( v );

		pipeline.Draw( itList );
	}

private:
	Pipeline pipeline;
	const IndexedTriangleList<Vertex> itList;

	float theta_x = 0.0f;
	float theta_y = 0.0f;
	float theta_z = 0.0f;
	float zOffset = 2.0f;

	float seconds = 0.0f;
};