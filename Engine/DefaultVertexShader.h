#pragma once

#include "Mat3.h"
#include "Vec3.h"

template <typename Vertex>
class DefaultVertexShader
{
public:
	typedef Vertex Output;
public:
	void BindRotation( const Mat3& rotation_in )
	{
		rotation = rotation_in;
	}
	void BindTranslation( const Vec3& translation_in )
	{
		translation = translation_in;
	}

	Output operator()( const Vertex& in )
	{
		Vec3 pos = in.pos * rotation + translation;
		return Output( pos, in );
	}
private:
	Mat3 rotation = Mat3::Identity();
	Vec3 translation;
};