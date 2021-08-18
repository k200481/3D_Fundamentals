#pragma once
#pragma once

#include "CubeScene.h"
#include "Pipeline.h"
#include "SpecularPhongEffect.h"
#include "SolidColorEffect.h"
#include "Sphere.h"

class SpecularPhongScene : public CubeScene
{
	typedef Pipeline<SolidColorEffect> LiPipeline;
	typedef LiPipeline::Vertex LiVertex;

	typedef Pipeline<SpecularPhongEffect> Pipeline;
	typedef Pipeline::Vertex Vertex;
public:
	SpecularPhongScene(Graphics& gfx, IndexedTriangleList<Vertex> itlist_in)
		:
		pZBuf(std::make_shared<ZBuffer>(Graphics::ScreenWidth, Graphics::ScreenHeight)),
		pipeline(gfx, pZBuf),
		liPipeline(gfx, pZBuf),
		itlist(std::move(itlist_in)),
		li_itlist(Sphere::GetPlain<LiVertex>(0.05f, 30, 30))
	{
		itlist.AdjustToTrueCenter();
		zOffset = itlist.GetRadius() * 1.6f;

		for (auto& v : li_itlist.vertices)
		{
			v.color = Colors::White;
		}
	}

	virtual void UpdateScene(Mouse& mouse, const Keyboard& kbd, float dt) override
	{
		CubeScene::UpdateScene(mouse, kbd, dt);

		const Vec2 mouseScreenPos = Vec2(mouse.GetPos());
		lightPos.x = mouseScreenPos.x / (Graphics::ScreenWidth / 2) - 1.0f;
		lightPos.y = -mouseScreenPos.y / (Graphics::ScreenHeight / 2) + 1.0f;

		if (kbd.KeyIsPressed(VK_NUMPAD8))
		{
			lightPos.z += 0.1f;
		}
		else if (kbd.KeyIsPressed(VK_NUMPAD2))
		{
			lightPos.z -= 0.1f;
		}
	}

	virtual void Render() override
	{
		pipeline.BeginScene();

		const auto proj = Mat4::Projection(2.0f, 2.0f, 1.0f, 10.0f);

		pipeline.effect.vs.BindWorld(
			Mat4::RotationX(theta_x) * 
			Mat4::RotationY(theta_y) * 
			Mat4::RotationZ(theta_z) * 
			Mat4::Translation(0.0f, 0.0f, zOffset)
		);
		pipeline.effect.vs.BindProjection(proj);
		pipeline.effect.ps.SetLightPosition(lightPos);

		liPipeline.effect.vs.BindWorld(
			Mat4::Translation(lightPos.x, lightPos.y, lightPos.z)
		);
		liPipeline.effect.vs.BindProjection(proj);

		pipeline.Draw(itlist);
		liPipeline.Draw(li_itlist);
	}

private:
	std::shared_ptr<ZBuffer> pZBuf;

	Pipeline pipeline;
	LiPipeline liPipeline;

	IndexedTriangleList<Vertex> itlist;
	IndexedTriangleList<LiVertex> li_itlist;

	Vec3 lightPos = { 0.0f, 0.0f, 1.0f };
};