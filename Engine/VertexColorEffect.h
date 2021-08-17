#pragma once

#include "Vec3.h"
#include "Colors.h"
#include "Mat.h"

class VertexColorEffect
{
public:
	// vertex class for pipeline
	class Vertex
	{
	public:
		// doesn't really need any operations to be defined since it's never interpolated
	public:
		Vec3 pos;
	};

public:
	// shaders

	// returns the color stored at the given vertex
	class PixelShader
	{
	public:
		template <class Input>
		Color operator()(const Input& v)
		{
			return Color( v.color );
		}
	};
	// assigns color value to vertex based on it's position
	class VertexShader
	{
	public:
		// VSOut for the pipeline
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
			Output(const Vec3& pos, const Vec3& c)
				:
				color(c),
				pos(pos)
			{}
			Output& operator+=(const Output& rhs)
			{
				pos += rhs.pos;
				color += rhs.color;
				return *this;
			}
			Output operator+(const Output& rhs) const
			{
				return Output(*this) += rhs;
			}
			Output& operator-=(const Output& rhs)
			{
				pos -= rhs.pos;
				color -= rhs.color;
				return *this;
			}
			Output operator-(const Output& rhs) const
			{
				return Output(*this) -= rhs;
			}
			Output& operator*=(float rhs)
			{
				pos *= rhs;
				color *= rhs;
				return *this;
			}
			Output operator*(float rhs) const
			{
				return Output(*this) *= rhs;
			}
			Output& operator/=(float rhs)
			{
				pos /= rhs;
				color /= rhs;
				return *this;
			}
			Output operator/(float rhs) const
			{
				return Output(*this) /= rhs;
			}
		public:
			Vec3 pos;
			Vec3 color;
		};

	public:
		// functions
		void BindRotation( const Mat3& rotation_in )
		{
			rotation = rotation_in;
		}
		void BindTranslation( const Vec3& translation_in )
		{
			translation = translation_in;
		}

		Output operator()( const Vertex& v )
		{
			const Vec3 pos = v.pos * rotation + translation;
			return Output( pos, Vec3( abs( pos.x ), abs( pos.y ), abs( pos.z ) ) * 255.0f );
		}

	private:
		Mat3 rotation = Mat3::Identity();
		Vec3 translation;
	};
	// default required by the pipeline, does nothing
	typedef DefaultGeometryShader<VertexShader::Output> GeometryShader;

public:
	PixelShader ps;
	VertexShader vs;
	GeometryShader gs;
};