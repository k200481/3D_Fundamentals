#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"

class Scene
{
public:
	virtual  void UpdateScene( Mouse& mouse, const Keyboard& kbd, float dt ) = 0;
	virtual void Render() = 0;
	virtual ~Scene() = default;
};