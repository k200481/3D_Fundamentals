#pragma once

#include "Vec3.h"
#include "Vec4.h"

template <typename T, size_t S>
class _Mat
{
public:
	_Mat& operator=(const _Mat& rhs)
	{
		memcpy(elements, rhs.elements, sizeof(elements));
		return *this;
	}
	_Mat& operator*=(T rhs)
	{
		for (auto& row : elements)
		{
			for (T& e : row)
			{
				e *= rhs;
			}
		}
		return *this;
	}
	_Mat operator*(T rhs) const
	{
		_Mat<T,S> result = *this;
		return result *= rhs;
	}
	_Mat operator*(const _Mat& rhs) const
	{
		_Mat<T,S> result;
		for (size_t j = 0; j < S; j++)
		{
			for (size_t k = 0; k < S; k++)
			{
				T sum = (T)0.0;
				for (size_t i = 0; i < S; i++)
				{
					sum += elements[j][i] * rhs.elements[i][k];
				}
				result.elements[j][k] = sum;
			}
		}
		return result;
	}
	
	static _Mat Identity()
	{
		if constexpr (S == 3)
		{
			return {
				(T)1.0,(T)0.0,(T)0.0,
				(T)0.0,(T)1.0,(T)0.0,
				(T)0.0,(T)0.0,(T)1.0
			};
		}
		else if constexpr (S == 4)
		{
			return {
				(T)1.0,(T)0.0,(T)0.0,(T)0.0,
				(T)0.0,(T)1.0,(T)0.0,(T)0.0,
				(T)0.0,(T)0.0,(T)1.0,(T)0.0,
				(T)0.0,(T)0.0,(T)0.0,(T)1.0,
			};
		}
		else
		{
			static_assert("Bad Matrix Size");
		}
	}
	
	static _Mat RotationZ(T theta)
	{
		const T cosTheta = (T)cos(theta);
		const T sinTheta = (T)sin(theta);

		if constexpr (S == 3)
		{
			return {
				 cosTheta,	sinTheta, (T)0.0,
				-sinTheta,	cosTheta, (T)0.0,
				 (T)0.0,	(T)0.0,	  (T)1.0
			};
		}
		else if constexpr (S == 4)
		{
			return {
				 cosTheta,	sinTheta, (T)0.0, (T)0.0,
				-sinTheta,	cosTheta, (T)0.0, (T)0.0,
				 (T)0.0,	(T)0.0,	  (T)1.0  (T)0.0,
				 (T)0.0,	(T)0.0,	  (T)0.0, (T)1.0
			};
		}
		else
		{
			static_assert("Bad Matrix Size");
		}
	}
	static _Mat RotationY(T theta)
	{
		const T cosTheta = (T)cos(theta);
		const T sinTheta = (T)sin(theta);

		if constexpr (S == 3)
		{
			return {
				cosTheta, (T)0.0, -sinTheta,
				(T)0.0,	  (T)1.0, (T)0.0,
				sinTheta, (T)0.0, cosTheta,
			};
		}
		else if constexpr (S == 4)
		{
			return {
				cosTheta, (T)0.0, -sinTheta, (T)0.0,
				(T)0.0,	  (T)1.0, (T)0.0,	 (T)0.0,
				sinTheta, (T)0.0, cosTheta,	 (T)0.0,
				(T)0.0,   (T)0.0, (T)0.0,	 (T)1.0
			};
		}
		else
		{
			static_assert("Bad Matrix Size");
		}
	}
	static _Mat RotationX(T theta)
	{
		const T cosTheta = (T)cos(theta);
		const T sinTheta = (T)sin(theta);
		
		if constexpr (S == 3)
		{
			return {
				(T)1.0,  (T)0.0,	(T)0.0,
				(T)0.0,  cosTheta,  sinTheta,
				(T)0.0, -sinTheta,  cosTheta,
			};
		}
		else if constexpr (S == 4)
		{
			return {
				(T)1.0,  (T)0.0,	(T)0.0,   (T)0.0,
				(T)0.0,  cosTheta,  sinTheta, (T)0.0,
				(T)0.0, -sinTheta,  cosTheta, (T)0.0,
				(T)0.0,   (T)0.0, (T)0.0,	 (T)1.0
			};
		}
		else
		{
			static_assert("Bad Matrix Size");
		}
	}

	static _Mat Translation( T x, T y, T z )
	{
		return {
			(T)0.0,(T)0.0,(T)0.0,(T)0.0,
			(T)0.0,(T)0.0,(T)0.0,(T)0.0,
			(T)0.0,(T)0.0,(T)0.0,(T)0.0,
			(T)x,  (T)y,  (T)z,  (T)1.0
		};
	}
	
	static _Mat Scaling(T factor)
	{
		if constexpr (S == 3)
		{
			return {
				factor,(T)0.0,(T)0.0,
				(T)0.0,factor,(T)0.0,
				(T)0.0,(T)0.0,factor,
			};
		}
		else if constexpr (S == 4)
		{
			return {
				factor,(T)0.0,(T)0.0,(T)0.0,
				(T)0.0,factor,(T)0.0,(T)0.0,
				(T)0.0,(T)0.0,factor,(T)0.0,
				(T)0.0,(T)0.0,(T)0.0,(T)1.0,
			};
		}
		else
		{
			static_assert("Bad Matrix Size");
		}
	}

public:
	// [ row ][ col ]
	T elements[S][S];
};


typedef _Mat<float, 3> Mat3;
typedef _Mat<float, 4> Mat4;
typedef _Mat<double, 3> Mad3;
typedef _Mat<double, 4> Mad4;

template<typename T>
_Vec3<T>& operator*=(_Vec3<T>& lhs, const _Mat<T, 3>& rhs)
{
	return lhs = lhs * rhs;
}

template<typename T>
_Vec3<T> operator*(const _Vec3<T>& lhs, const _Mat<T, 3>& rhs)
{
	return {
		lhs.x * rhs.elements[0][0] + lhs.y * rhs.elements[1][0] + lhs.z * rhs.elements[2][0],
		lhs.x * rhs.elements[0][1] + lhs.y * rhs.elements[1][1] + lhs.z * rhs.elements[2][1],
		lhs.x * rhs.elements[0][2] + lhs.y * rhs.elements[1][2] + lhs.z * rhs.elements[2][2]
	};
}

template<typename T>
_Vec4<T>& operator*=(_Vec4<T>& lhs, const _Mat<T, 4>& rhs)
{
	return lhs = lhs * rhs;
}

template<typename T>
_Vec4<T> operator*(const _Vec4<T>& lhs, const _Mat<T, 4>& rhs)
{
	return {
		lhs.x * rhs.elements[0][0] + lhs.y * rhs.elements[1][0] + lhs.z * rhs.elements[2][0] + lhs.w * rhs.elements[3][0],
		lhs.x * rhs.elements[0][1] + lhs.y * rhs.elements[1][1] + lhs.z * rhs.elements[2][1] + lhs.w * rhs.elements[3][1],
		lhs.x * rhs.elements[0][2] + lhs.y * rhs.elements[1][2] + lhs.z * rhs.elements[2][2] + lhs.w * rhs.elements[3][2],
		lhs.x * rhs.elements[0][3] + lhs.y * rhs.elements[1][3] + lhs.z * rhs.elements[2][3] + lhs.w * rhs.elements[3][3],
	};
}