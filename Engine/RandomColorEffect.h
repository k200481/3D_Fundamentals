#pragma once

#include "Vec3.h"
#include "Colors.h"
#include "DefaultVertexShader.h"
#include "DefaultGeometryShader.h"
#include <random>

class RandomColorEffect
{
public:
	// 
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

public:
	class PixelShader
	{
	public:
		template <class Input>
		Color operator()( const Input& in )
		{
			return in.c;
		}
	};
	
	typedef DefaultVertexShader<Vertex> VertexShader;

	//typedef DefaultGeometryShader<Vertex> GeometryShader;
	class GeometryShader
	{
	public:
		class Output
		{
		public:
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
			Vec3 pos;
			Color c;
		};
	public:
		GeometryShader()
			:
			rng(std::random_device()())
		{
		}
		Triangle<Output> operator()(const Vertex& v0, const Vertex& v1, const Vertex& v2, size_t triangle_id)
		{
			//const size_t i = rng() % colors.size();
			const size_t i = triangle_id % colors.size();
			const Color c = colors[i];
			return { { v0.pos, c }, { v1.pos, c }, { v2.pos, c } };
		}
	private:
		std::vector<Color> colors = { Colors::Yellow, Colors::Blue, Colors::Green, Colors::Red, Colors::Magenta, Colors::Gray };
		std::mt19937 rng;
	};

public:
	PixelShader ps;
	VertexShader vs;
	GeometryShader gs;
};
