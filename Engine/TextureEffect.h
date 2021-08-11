#pragma once

#include "Vec3.h"
#include "Surface.h"
#include <cmath>
#include <string>

#include "DefaultVertexShader.h"

class TextureEffect
{
public:
	// vertex class for pipeline
	class Vertex
	{
	public:
		Vertex() = default;
		Vertex(const Vec3& pos)
			:
			pos(pos)
		{}
		// this enables template functions clone a vertex
		// while changing the pos only
		Vertex(const Vec3& pos, const Vertex& src)
			:
			t(src.t),
			pos(pos)
		{}
		Vertex(const Vec3& pos, const Vec2& t)
			:
			t(t),
			pos(pos)
		{}
		Vertex& operator+=(const Vertex& rhs)
		{
			pos += rhs.pos;
			t += rhs.t;
			return *this;
		}
		Vertex operator+(const Vertex& rhs) const
		{
			return Vertex(*this) += rhs;
		}
		Vertex& operator-=(const Vertex& rhs)
		{
			pos -= rhs.pos;
			t -= rhs.t;
			return *this;
		}
		Vertex operator-(const Vertex& rhs) const
		{
			return Vertex(*this) -= rhs;
		}
		Vertex& operator*=(float rhs)
		{
			pos *= rhs;
			t *= rhs;
			return *this;
		}
		Vertex operator*(float rhs) const
		{
			return Vertex(*this) *= rhs;
		}
		Vertex& operator/=(float rhs)
		{
			pos /= rhs;
			t /= rhs;
			return *this;
		}
		Vertex operator/(float rhs) const
		{
			return Vertex(*this) /= rhs;
		}
	public:
		Vec3 pos;
		Vec2 t;
	};
	typedef DefaultVertexShader<Vertex> VertexShader;
public:
	// 
	class PixelShader
	{
	public:
		Color operator()(const Vertex& v)
		{
			return pTex->GetPixel(
				(int)std::min(v.t.x * texWidth, texClamp_x),
				(int)std::min(v.t.y * texHeight, texClamp_y)
			);
		}
		void BindTexture(std::wstring filename)
		{
			pTex = std::make_unique<Surface>( Surface::FromFile( filename ) );
			texWidth = (float)pTex->GetWidth();
			texHeight = (float)pTex->GetHeight();
			texClamp_x = texWidth - 1.0f;
			texClamp_y = texHeight - 1.0f;
		}
	private:
		std::unique_ptr<Surface> pTex;
		float texWidth = 0;
		float texHeight = 0;
		float texClamp_x = 0;
		float texClamp_y = 0;
	};

public:
	PixelShader ps;
	VertexShader vs;
};
