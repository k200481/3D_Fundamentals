#pragma once

#include "Vec3.h"
#include "Mat3.h"
#include "Colors.h"
#include "Surface.h"

class WaveEffect
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

	class VertexShader
	{
	public:
		typedef Vertex Output;
		void BindRotation( const Mat3& rotation_in )
		{
			rotation = rotation_in;
		}
		void BindTranslation( const Vec3& translation_in )
		{
			translation = translation_in;
		}
		void SetTime( float time_in )
		{
			time = time_in;
		}

		Output operator()( const Vertex& in )
		{
			Vec3 pos = in.pos * rotation + translation;
			pos.y += a * (float)std::sin( pos.x * k + w * time );
			return Output( pos, in.t );
		}
	private:
		Mat3 rotation;
		Vec3 translation;
		float time;
		// time period of wave in seconds
		static constexpr float freq = 1.0f;
		static constexpr float waveLen = 0.5f;
		static constexpr float w = 2.0f * PI * freq;
		static constexpr float k = 2.0f * PI / waveLen;
		static constexpr float a = 0.05f;
	};

public:
	PixelShader ps;
	VertexShader vs;
};