#pragma once

#include "Vec3.h"
#include "Mat.h"
#include "Colors.h"

#include "DefaultGeometryShader.h"

class PhongPointEffect
{
public:
	// basic vertex
	class Vertex
	{
	public:
		Vertex() = default;
		Vertex(const Vec3& pos)
			:
			pos(pos)
		{
		}
		Vertex(const Vec3& pos, const Vec3& n)
			:
			pos(pos),
			n(n)
		{
		}
		Vertex(const Vec3& pos, const Vertex& src)
			:
			pos(pos),
			n(src.n)
		{
		}
		Vertex& operator+=(const Vertex& rhs)
		{
			pos += rhs.pos;
			return *this;
		}
		Vertex& operator-=(const Vertex& rhs)
		{
			pos -= rhs.pos;
			return *this;
		}
		Vertex& operator*=(float rhs)
		{
			pos *= rhs;
			return *this;
		}
		Vertex& operator/=(float rhs)
		{
			pos /= rhs;
			return *this;
		}
		Vertex operator+(const Vertex& rhs) const
		{
			return Vertex(*this) += rhs;
		}
		Vertex operator-(const Vertex& rhs) const
		{
			return Vertex(*this) -= rhs;
		}
		Vertex operator*(float rhs) const
		{
			return Vertex(*this) *= rhs;
		}
		Vertex operator/(float rhs) const
		{
			return Vertex(*this) /= rhs;
		}
	public:
		Vec3 pos;
		Vec3 n;
	};
public:
	// shaders

	// uses normals, world position and light information to do per-pixel shading
	class PixelShader
	{
	public:
		void SetLightPosition(const Vec3& pos_in)
		{
			lightPos = pos_in;
		}
		void SetLightColor(Color c)
		{
			lightCol = Vec3(c) / 255.0f;
		}
		void SetAmbientLight(Color c)
		{
			ambient = Vec3(c) / 255.0f;
		}
		void SetMaterialColor(Color c)
		{
			color = Vec3(c) / 255.0f;
		}
		void SetQuadraticAttennuation(float at_in)
		{
			quadraticAttennuation = at_in;
		}
		void SetLinearAttennuation(float at_in)
		{
			linearAttennuation = at_in;
		}
		void SetConstantAttennuation(float at_in)
		{
			constantAttennuation = at_in;
		}

		template <class Input>
		Color operator()(const Input& in)
		{
			// vetor from vertex to light source
			const auto vecToLS = lightPos - in.worldPos;
			// length of the vector
			const auto len = vecToLS.Len();
			// direction of light ray
			const auto lightDir = vecToLS / len;

			// the total attennuation of the light ray
			const auto attennuation = 1.0f / (quadraticAttennuation * sq(len) + linearAttennuation * len + constantAttennuation);
			// diffused light
			const auto d = lightCol * attennuation * std::fmax(0.0f, in.n.GetNormalized() * lightDir);
			// the final color of the vertex
			const auto c = color.GetHadamard(d + ambient).Saturate() * 255.0f;
			return Color(c);
		}
	private:
		// light information
		Vec3 lightPos = { 0.0f, -1.0f, 1.0f };
		// the 'color' of the light
		Vec3 lightCol = { 1.0f, 1.0f, 1.0f };
		// the 'color' of the ambient light
		Vec3 ambient = { 0.1f, 0.1f, 0.1f };
		// material color
		Vec3 color = Vec3{ 0.85f, 0.9f, 0.85f } ;

		float quadraticAttennuation = 1.0f;
		float linearAttennuation = 2.619f;
		float constantAttennuation = 0.382;
	};
	// rotates and translates the position, rotates the normals
	// outputs a vertex that separately stores world position
	class VertexShader
	{
	public:
		// output vertex
		class Output
		{
		public:
			Output() = default;
			Output(const Vec3 & pos)
				:
				pos(pos)
			{
			}
			Output(const Vec3 & pos, const Vec3 & n, const Vec3& worldPos)
				:
				pos(pos),
				n(n),
				worldPos(worldPos)
			{
			}
			Output(const Vec3 & pos, const Output & src)
				:
				pos(pos),
				n(src.n),
				worldPos(src.worldPos)
			{
			}
			Output& operator+=(const Output & rhs)
			{
				pos += rhs.pos;
				n += rhs.n;
				worldPos += rhs.worldPos;
				return *this;
			}
			Output& operator-=(const Output & rhs)
			{
				pos -= rhs.pos;
				n -= rhs.n;
				worldPos -= rhs.worldPos;
				return *this;
			}
			Output& operator*=(float rhs)
			{
				pos *= rhs;
				n *= rhs;
				worldPos *= rhs;
				return *this;
			}
			Output& operator/=(float rhs)
			{
				pos /= rhs;
				n /= rhs;
				worldPos /= rhs;
				return *this;
			}
			Output operator+(const Output & rhs) const
			{
				return Output(*this) += rhs;
			}
			Output operator-(const Output & rhs) const
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
			Vec3 n;
			Vec3 worldPos;
		};
	public:
		void BindRotation(const Mat3& rotation_in)
		{
			rotation = rotation_in;
		}
		void BindTranslation(const Vec3& translation_in)
		{
			translation = translation_in;
		}

		Output operator()(const Vertex& in)
		{
			const auto pos = in.pos * rotation + translation;
			return { pos, in.n * rotation, pos };
		}

	private:
		Mat3 rotation = Mat3::Identity();
		Vec3 translation;
	};
	// default does nothing
	typedef DefaultGeometryShader<VertexShader::Output> GeometryShader;
public:
	PixelShader ps;
	VertexShader vs;
	GeometryShader gs;
};