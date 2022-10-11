#pragma once

#include "rogll/window.h"
#include "glfw3.h"
#include "imgui_impl_glfw.h"

namespace GUI
{
	void Initialize(ROGLL::Window* window);
	void PrePass();
	void Draw();
}
