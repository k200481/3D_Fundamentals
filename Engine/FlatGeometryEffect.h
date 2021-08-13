#pragma once

#include "Vec3.h"
#include "Colors.h"
#include "DefaultVertexShader.h"
#include "Triangle.h"

class FlatGeometryEffect
{
public:
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
			pos(pos)
		{}
		Vertex& operator+=(const Vertex& rhs)
		{
			pos += rhs.pos;
			return *this;
		}
		Vertex operator+(const Vertex& rhs) const
		{
			return Vertex(*this) += rhs;
		}
		Vertex& operator-=(const Vertex& rhs)
		{
			pos -= rhs.pos;
			return *this;
		}
		Vertex operator-(const Vertex& rhs) const
		{
			return Vertex(*this) -= rhs;
		}
		Vertex& operator*=(float rhs)
		{
			pos *= rhs;
			return *this;
		}
		Vertex operator*(float rhs) const
		{
			return Vertex(*this) *= rhs;
		}
		Vertex& operator/=(float rhs)
		{
			pos /= rhs;
			return *this;
		}
		Vertex operator/(float rhs) const
		{
			return Vertex(*this) /= rhs;
		}
	public:
		Vec3 pos;
	};

	// shaders

	// returns the color stored in the vertex
	class PixelShader
	{
	public:
		template <class Input>
		Color operator()(const Input& in) const
		{
			return in.color;
		}
	};
	// default required by pipeline, does nothing
	typedef DefaultVertexShader<Vertex> VertexShader;
	// 
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
				color(src.color),
				pos(pos)
			{}
			Output(const Vec3& pos, Color c)
				:
				color(c),
				pos(pos)
			{}
			Output& operator+=(const Output& rhs)
			{
				pos += rhs.pos;
				return *this;
			}
			Output operator+(const Output& rhs) const
			{
				return Output(*this) += rhs;
			}
			Output& operator-=(const Output& rhs)
			{
				pos -= rhs.pos;
				return *this;
			}
			Output operator-(const Output& rhs) const
			{
				return Output(*this) -= rhs;
			}
			Output& operator*=(float rhs)
			{
				pos *= rhs;
				return *this;
			}
			Output operator*(float rhs) const
			{
				return Output(*this) *= rhs;
			}
			Output& operator/=(float rhs)
			{
				pos /= rhs;
				return *this;
			}
			Output operator/(float rhs) const
			{
				return Output(*this) /= rhs;
			}
		public:
			Vec3 pos;
			Color color;
		};
	public:
		void SetLightDirection( const Vec3& dir_in )
		{
			lightDir = dir_in;
		}
		void SetLightColor( Color c )
		{
			lightCol = Vec3( c ) / 255.0f;
		}
		void SetAmbientLight( Color c )
		{
			ambient = Vec3( c ) / 255.0f;
		}
		void SetMaterialColor( Color c )
		{
			color = Vec3( c ) / 255.0f;
		}

		Triangle<Output> operator()( const Vertex& v0, const Vertex& v1, const Vertex& v2, size_t triangle_id ) const
		{
			// plane normal
			const auto crossP = (( v1.pos - v0.pos ) % ( v2.pos - v0.pos )).GetNormalized();
			const auto d = lightCol * std::fmax( 0.0f, -crossP * lightDir );
			Color c = Color( color.GetHadamard( d + ambient ).Saturate() * 255.0f );

			return { { v0.pos, c }, { v1.pos, c }, { v2.pos, c } };
		}

	private:
		// light information
		Vec3 lightDir = { 0.0f, 0.0f, 1.0f };
		// the 'color' of the light
		Vec3 lightCol = { 1.0f, 1.0f, 1.0f };
		// the 'color' of the ambient light
		Vec3 ambient = { 0.1f, 0.1f, 0.1f };
		// material color
		Vec3 color = { 1.0f, 1.0f, 1.0f };
	};
public:
	PixelShader ps;
	VertexShader vs;
	GeometryShader gs;
};