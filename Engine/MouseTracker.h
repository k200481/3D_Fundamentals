#pragma once

#include "Mouse.h"

class MouseTracker
{
public:

	void Engage(Vei2& pos)
	{
		base = Vec2(pos);
		engaged = true;
	}
	void Disengage()
	{
		engaged = false;
	}
	bool IsEngaged() const
	{
		return engaged;
	}
	Vec2 Move(const Vei2& pos)
	{
		if (engaged)
		{
			const auto old = base;
			base = Vec2(pos);
			return old - base;
		}
		else
		{
			return Vec2(0.0f, 0.0f);
		}
	}
private:
	Vec2 base;
	bool engaged = false;
};