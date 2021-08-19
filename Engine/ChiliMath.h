/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	ChiliMath.h																			  *
*	Copyright 2016 PlanetChili <http://www.planetchili.net>								  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#pragma once

#include <math.h>
#include <cmath>

constexpr float PI = 3.14159265f;
constexpr double PI_D = 3.1415926535897932;

template <typename T>
inline auto sq( const T& x )
{
	return x * x;
}

template <typename T>
inline auto wrap_angle( T angle )
{
	T moddedAngle = (T)fmod( angle, (T)2.0 * (T)PI_D );
	return ( moddedAngle > (T)PI_D ) ? ( moddedAngle - (T)2.0 * (T)PI_D ) : moddedAngle;
}

template <typename T>
inline T interpolate( T src, T dst, float alpha )
{
	return { src + (dst - src) * alpha };
}

template <typename T>
constexpr inline auto to_rad(T degree)
{
	return degree * (T)PI_D / 180.0f;
}