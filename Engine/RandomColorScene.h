#pragma once

#include "CubeScene.h"
#include "Pipeline.h"
#include "RandomColorEffect.h"
#include "Plane.h"
#include "Sphere.h"

class RandomColorScene : public CubeScene
{
	typedef Pipeline<RandomColorEffect> Pipe;
	typedef Pipe::Vertex Vertex;
public:

	RandomColorScene(Graphics& gfx)
		:
		pipe(gfx),
		itlist(Sphere::GetPlain<Vertex>(1.0f, 40, 40))
	{
		zOffset = itlist.GetRadius() * 2.0f;
	}

	virtual void Render() override
	{
		pipe.BeginScene();

		Mat3 m = Mat3::RotationX(theta_x) * Mat3::RotationY(theta_y) * Mat3::RotationZ(theta_z);
		Vec3 v = { 0.0f, 0.0f, zOffset };

		pipe.effect.vs.BindRotation(m);
		pipe.effect.vs.BindTranslation(v);

		pipe.Draw(itlist);
	}

private:
	Pipe pipe;
	IndexedTriangleList<Vertex> itlist;
};