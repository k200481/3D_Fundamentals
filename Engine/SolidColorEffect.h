#pragma once

#include "Vec3.h"
#include "Mat.h"
#include "Graphics.h"
#include "DefaultGeometryShader.h"

class SolidColorEffect
{
public:
	// vertex class for pipeline
	class Vertex
	{
	public:
		Vertex() = default;
		Vertex(const Vec3 & pos)
			:
			pos(pos)
		{}
		// this enables template functions clone a vertex
		// while changing the pos only
		Vertex(const Vec3 & pos, const Vertex & src)
			:
			color(src.color),
			pos(pos)
		{}
		Vertex(const Vec3 & pos, Color c)
			:
			color(c),
			pos(pos)
		{}
		Vertex& operator+=(const Vertex & rhs)
		{
			pos += rhs.pos;
			return *this;
		}
		Vertex operator+(const Vertex & rhs) const
		{
			return Vertex(*this) += rhs;
		}
		Vertex& operator-=(const Vertex & rhs)
		{
			pos -= rhs.pos;
			return *this;
		}
		Vertex operator-(const Vertex & rhs) const
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
		Color color;
	};
	
public:
	// shaders

	// returs color stored at the vertex
	class PixelShader
	{
	public:
		template <class Input>
		Color operator()( const Input& in )
		{
			return in.c;
		}
	};

	class VertexShader
	{
	public:
		// output vertex
		class Output
		{
		public:
			Output() = default;
			Output(const Vec4& pos)
				:
				pos(pos)
			{
			}
			Output(const Vec4& pos, Color c)
				:
				pos(pos),
				c(c)
			{
			}
			Output(const Vec4& pos, const Output& src)
				:
				pos(pos),
				c(src.c)
			{
			}
			Output& operator+=(const Output& rhs)
			{
				pos += rhs.pos;
				return *this;
			}
			Output& operator-=(const Output& rhs)
			{
				pos -= rhs.pos;
				return *this;
			}
			Output& operator*=(float rhs)
			{
				pos *= rhs;
				return *this;
			}
			Output& operator/=(float rhs)
			{
				pos /= rhs;
				return *this;
			}
			Output operator+(const Output& rhs) const
			{
				return Output(*this) += rhs;
			}
			Output operator-(const Output& rhs) const
			{
				return Output(*this) -= rhs;
			}
			Output operator*(float rhs) const
			{
				return Output(*this) *= rhs;
			}
			Output operator/(float rhs) const
			{
				return Output(*this) /= rhs;
			}
		public:
			Vec4 pos;
			Color c;
		};

	public:
		void BindWorld(const Mat4& world_in)
		{
			world = world_in;
			worldProj = world * proj;
		}
		void BindProjection(const Mat4& proj_in)
		{
			proj = proj_in;
			worldProj = world * proj;
		}
		auto GetProjection() const
		{
			return proj;
		}

		Output operator()(const Vertex& in)
		{
			return { Vec4(in.pos) * worldProj, in.color };
		}

	private:
		Mat4 world = Mat4::Identity();
		Mat4 proj = Mat4::Identity();
		Mat4 worldProj = Mat4::Identity();
	};

	// default required by the pipeline, does nothing
	typedef DefaultGeometryShader<VertexShader::Output> GeometryShader;

public:
	PixelShader ps;
	VertexShader vs;
	GeometryShader gs;
};