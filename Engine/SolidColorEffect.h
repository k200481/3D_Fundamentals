#pragma once

#include "Vec3.h"
#include "Graphics.h"
#include "DefaultVertexShader.h"

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
	typedef DefaultVertexShader<Vertex> VertexShader;

public:
	//
	class PixelShader
	{
	public:
		Color operator()( const Vertex& v )
		{
			return v.color;
		}
	};

public:
	PixelShader ps;
	VertexShader vs;
};