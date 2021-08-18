#pragma once

#include "Scene.h"
#include "Mat.h"
#include "Cube.h"

class CubeScene : public Scene
{
public:
	CubeScene()
		:
		c( 1.0f )
	{
	}
	virtual ~CubeScene() = default;

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
	}

protected:
	Cube c;

	float theta_x = 0.0f;
	float theta_y = 0.0f;
	float theta_z = 0.0f;

	float zOffset = 2.0f;
};