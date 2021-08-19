#pragma once
#pragma once

#include "CubeScene.h"
#include "Pipeline.h"
#include "SpecularPhongEffect.h"
#include "SolidColorEffect.h"
#include "Sphere.h"
#include "MouseTracker.h"

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
		// fill in the colors of the light source
		for (auto& v : li_itlist.vertices)
		{
			v.color = Colors::White;
		}

		const auto proj = Mat4::ProjectionHFOV(hfov, aspectRatio, 0.5f, 4.0f);
		pipeline.effect.vs.BindProjection(proj);
		liPipeline.effect.vs.BindProjection(proj);
	}

	virtual void UpdateScene(Mouse& mouse, const Keyboard& kbd, float dt) override
	{
		// adjust position
		if (kbd.KeyIsPressed('W'))
		{
			camPos += Vec4({ 0.0f,0.0f,1.0f }) * !camInvRot * dt;
		}
		else if (kbd.KeyIsPressed('S'))
		{
			camPos -= Vec4({ 0.0f,0.0f,1.0f }) * !camInvRot * dt;
		}
		if (kbd.KeyIsPressed('D'))
		{
			camPos += Vec4({ 1.0f,0.0f,0.0f }) * !camInvRot * dt;
		}
		else if (kbd.KeyIsPressed('A'))
		{
			camPos -= Vec4({ 1.0f,0.0f,0.0f }) * !camInvRot * dt;
		}
		if (kbd.KeyIsPressed(VK_SPACE))
		{
			camPos += Vec4({ 0.0f,1.0f,0.0f }) * !camInvRot * dt;
		}
		else if (kbd.KeyIsPressed('C'))
		{
			camPos -= Vec4({ 0.0f,1.0f,0.0f }) * !camInvRot * dt;
		}

		
		Mouse::Event e;
		while ( (e = mouse.Read()).GetType() != Mouse::Event::Invalid )
		{
			switch (e.GetType())
			{
			case Mouse::Event::LPress:
				mt.Engage(e.GetPos());
				break;
			case Mouse::Event::LRelease:
				mt.Disengage();
				break;
			case Mouse::Event::Move:
				if (mt.IsEngaged()) {
					const auto delta = mt.Move(e.GetPos());
					camInvRot = camInvRot
						* Mat4::RotationY(-delta.x * hTrack)
						* Mat4::RotationX(-delta.y * vTrack);
				}
				break;
			}
		}
	}

	virtual void Render() override
	{
		pipeline.BeginScene();

		const auto camInvTrans = Mat4::Translation(-camPos);
		const auto camInv = camInvTrans * camInvRot;

		// bind world transformations
		pipeline.effect.vs.BindWorld(
			Mat4::RotationX(theta_x) * 
			Mat4::RotationY(theta_y) * 
			Mat4::RotationZ(theta_z) * 
			Mat4::Translation(pos)
		);
		liPipeline.effect.vs.BindWorld(
			Mat4::Translation(lightPos)
		);
		// bind camera transformations
		pipeline.effect.vs.BindView(camInv);

		// update light source position in the world
		pipeline.effect.ps.SetLightPosition(lightPos);

		// draw
		pipeline.Draw(itlist);
		liPipeline.Draw(li_itlist);
	}

private:
	std::shared_ptr<ZBuffer> pZBuf;

	Pipeline pipeline;
	LiPipeline liPipeline;

	IndexedTriangleList<Vertex> itlist;
	IndexedTriangleList<LiVertex> li_itlist;

	MouseTracker mt;

	Vec3 pos = { 0.0f, 0.0f, 2.0f };
	Vec4 lightPos = { 0.5f,-0.5f,0.55f };
	Vec4 camPos = { 0.0f, 0.0f, 0.0f };
	Mat4 camInvRot = Mat4::Identity();

	float camThetaY = 0.0f;
	float camThetaX = 0.0f;

	static constexpr float camSpeed = 1.0f;
	static constexpr float hfov = 100.0f;
	static constexpr float aspectRatio = 1.33333f;
	static constexpr float vfov = hfov * aspectRatio;

	static constexpr float hTrack = to_rad(hfov) / (float)Graphics::ScreenWidth;
	static constexpr float vTrack = to_rad(vfov) / (float)Graphics::ScreenHeight;
};