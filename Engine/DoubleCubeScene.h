#pragma once

#include "Scene.h"
#include "Pipeline.h"
#include "SolidColorEffect.h"
#include "Cube.h"

class DoubleCubeScene : public Scene
{
	typedef Pipeline<SolidColorEffect> Pipeline;
	typedef SolidColorEffect::Vertex Vertex;
public:
	DoubleCubeScene( Graphics& gfx )
		:
		pipeline( gfx )
	{
	}

	// updates both cubes using user input
	virtual void UpdateScene( Mouse& mouse, const Keyboard& kbd, float dt ) override
	{
		const float dTheta = PI / 2;

		if (kbd.KeyIsPressed(VK_UP))
		{
			zOffset1 += 0.1f;
		}
		else if (kbd.KeyIsPressed(VK_DOWN))
		{
			zOffset1 -= 0.1f;
		}

		if (kbd.KeyIsPressed('Q'))
		{
			theta_z1 += dt * dTheta;
		}
		else if (kbd.KeyIsPressed('E'))
		{
			theta_z1 -= dt * dTheta;
		}

		if (kbd.KeyIsPressed('W'))
		{
			theta_x1 += dt * dTheta;
		}
		else if (kbd.KeyIsPressed('S'))
		{
			theta_x1 -= dt * dTheta;
		}

		if (kbd.KeyIsPressed('A'))
		{
			theta_y1 += dt * dTheta;
		}
		else if (kbd.KeyIsPressed('D'))
		{
			theta_y1 -= dt * dTheta;
		}
		theta_x1 = wrap_angle(theta_x1);
		theta_y1 = wrap_angle(theta_y1);
		theta_z1 = wrap_angle(theta_z1);
	}
	// renders both cubes
	virtual void Render() override
	{
		pipeline.BeginScene();
		// the colors of the sides of the cube
		std::vector<Color> colors = {
			Colors::Yellow,
			Colors::Red,
			Colors::Blue,
			Colors::White,
			Colors::Green,
			Colors::Cyan
		};

		// get both triangle lists
		auto triangles1 = Cube::GetPlainIndependentFaces<Vertex>();
		auto triangles2 = Cube::GetPlainIndependentFaces<Vertex>();

		// get both matrices
		const Mat3 m1 = Mat3::RotationX(theta_x1) * Mat3::RotationY(theta_y1) * Mat3::RotationZ(theta_z1);
		const Mat3 m2 = Mat3::RotationX(-theta_x1) * Mat3::RotationY(-theta_y1) * Mat3::RotationZ(-theta_z1);

		// bind vertices of both cubes to colors
		for (size_t i = 0, e = triangles1.vertices.size(); i < e; i++)
		{
			triangles1.vertices[i].color = colors[i / 4];
			triangles2.vertices[i].color = colors[i / 4];
		}

		// draw the second cube
		pipeline.effect.vs.BindRotation(m2);
		pipeline.effect.vs.BindTranslation({ 0.0f, 0.0f, zOffset2 });
		pipeline.Draw(triangles2);

		// draw the first cube
		pipeline.effect.vs.BindRotation(m1);
		pipeline.effect.vs.BindTranslation({ -0.0f, 0.0f, zOffset1 });
		pipeline.Draw(triangles1);
	}

private:
	Pipeline pipeline;

	// cube 1
	float theta_x1 = 0.0f;
	float theta_y1 = 0.0f;
	float theta_z1 = 0.0f;
	float zOffset1 = 2.0f;
	
	// cube 2
	float zOffset2 = 2.0f;
};