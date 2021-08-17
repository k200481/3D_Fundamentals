#pragma once

#include "Vec3.h"
#include "Mat.h"
#include "Colors.h"
#include "DefaultGeometryShader.h"
#include <cmath>

class FlatVertexEffect
{
public:
	// vertex class input into the pipeline
	class Vertex
	{
	public:
		Vertex() = default;
		Vertex( const Vec3& pos, const Vec3& n )
			:
			pos( pos ),
			n( n )
		{
		}
		Vertex( const Vec3& pos, const Vertex& src )
			:
			pos( pos ),
			n( src.n )
		{
		}
		// no interpolation iirc
	public:
		Vec3 pos;
		// normal
		Vec3 n;
	};

public:
	// shaders

	// returns the color stored at the vertex
	class PixelShader
	{
	public:
		template <class Input>
		Color operator()( const Input& in )
		{
			return in.color;
		}
	};
	// assigns a color to the input vertex using vertex normal and a uniform light source
	class VertexShader
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
		// configuration

		void BindRotation( const Mat3& rotation_in )
		{
			rotation = rotation_in;
		}
		void BindTranslation( const Vec3& translation_in )
		{
			translation = translation_in;
		}
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

		Output operator()( const Vertex& v ) const
		{
			// calculate light intensity and get the diffused light 'color'
			const auto d = lightCol * std::fmax( 0.0f, ( -v.n * rotation ) * lightDir );
			//
			Color c = Color( color.GetHadamard( d + ambient ).Saturate() * 255.0f );
			return { v.pos * rotation + translation, c };
		}

	private:
		// vertex transformations
		Mat3 rotation = Mat3::Identity();
		Vec3 translation;
		// light information
		Vec3 lightDir = { 0.0f, 0.0f, 1.0f };
		// the 'color' of the light
		Vec3 lightCol = { 1.0f, 1.0f, 1.0f };
		// the 'color' of the ambient light
		Vec3 ambient = { 0.1f, 0.1f, 0.1f };
		// material color
		Vec3 color = { 1.0f, 1.0f, 1.0f };
	};
	// default, does nothing
	typedef DefaultGeometryShader<VertexShader::Output> GeometryShader;
public:
	PixelShader ps;
	VertexShader vs;
	GeometryShader gs;
};