#pragma once

#include "Vec3.h"
#include "Mat.h"
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
	// performs lookup on the texture using given coordinates to get pixel
	// clamps texture coordinates if out of range
	// applies lighting effect to the pixel
	class PixelShader
	{
	public:
		template <class Input>
		Color operator()(const Input& in)
		{
			Vec3 c = Vec3( pTex->GetPixel(
				(int)std::min(in.t.x * texWidth, texClamp_x),
				(int)std::min(in.t.y * texHeight, texClamp_y)
			) ) / 255.0f;

			return Color( c.GetHadamard( in.l ).Saturate() * 255.0f );
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
	// transforms given vertices to generate a wave-like effect
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
		Mat3 rotation = Mat3::Identity();
		Vec3 translation;
		float time = 0.0f;
		// time period of wave in seconds
		static constexpr float freq = 1.0f;
		static constexpr float waveLen = 0.5f;
		static constexpr float w = 2.0f * PI * freq;
		static constexpr float k = 2.0f * PI / waveLen;
		static constexpr float a = 0.05f;
	};
	// applies lighting effect to triangles 
	// based on the dot product of their normals and uniform light rays
	class GeometryShader
	{
	public:
		// the vertex output by this shader
		class Output
		{
		public:
			Output() = default;
			Output(const Vec3& pos)
				:
				pos(pos)
			{}
			// this enables template functions clone a vertex
			// while changing the pos only
			Output(const Vec3& pos, const Output& src)
				:
				pos(pos),
				t(src.t),
				l(l)
			{}
			Output(const Vec3& pos, const Vec2& t, const Vec3& l)
				:
				pos(pos),
				t(t),
				l(l)
			{}
			Output& operator+=(const Output& rhs)
			{
				pos += rhs.pos;
				t += rhs.t;
				return *this;
			}
			Output operator+(const Output& rhs) const
			{
				return Output(*this) += rhs;
			}
			Output& operator-=(const Output& rhs)
			{
				pos -= rhs.pos;
				t -= rhs.t;
				return *this;
			}
			Output operator-(const Output& rhs) const
			{
				return Output(*this) -= rhs;
			}
			Output& operator*=(float rhs)
			{
				pos *= rhs;
				t *= rhs;
				return *this;
			}
			Output operator*(float rhs) const
			{
				return Output(*this) *= rhs;
			}
			Output& operator/=(float rhs)
			{
				pos /= rhs;
				t /= rhs;
				return *this;
			}
			Output operator/(float rhs) const
			{
				return Output(*this) /= rhs;
			}
		public:
			Vec3 pos;
			Vec2 t;
			Vec3 l;
		};
	public:
		void SetLightDirection(const Vec3& dir_in)
		{
			lightDir = dir_in.GetNormalized();
		}
		void SetLightColor(Color c)
		{
			lightCol = Vec3(c) / 255.0f;
		}
		void SetAmbientLight(Color c)
		{
			ambient = Vec3(c) / 255.0f;
		}

		Triangle<Output> operator()(const Vertex& v0, const Vertex& v1, const Vertex& v2, size_t triangle_id) const
		{
			// plane normal
			const auto crossP = ((v1.pos - v0.pos) % (v2.pos - v0.pos)).GetNormalized();
			// dot product of plane normal and light rays
			const auto dotP = std::fmax(0.0f, -crossP * lightDir);
			// light intensity ( as a vector of the three primary colors )
			const auto l = lightCol * dotP + ambient;

			return { { v0.pos, v0.t, l }, { v1.pos, v1.t, l }, { v2.pos, v2.t, l } };
		}

	private:
		// light information
		Vec3 lightDir = Vec3{ -1.0f, 0.0f, 1.0f }.GetNormalized();
		// the 'color' of the light
		Vec3 lightCol = { 1.0f, 1.0f, 1.0f };
		// the 'color' of the ambient light
		Vec3 ambient = { 0.1f, 0.1f, 0.1f };
	};

public:
	PixelShader ps;
	VertexShader vs;
	GeometryShader gs;
};