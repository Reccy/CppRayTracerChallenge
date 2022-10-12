#pragma once
#include <string>
#include <glad.h>
#include <glfw3.h>

namespace ROGLL
{
	class Window
	{
	public:
		Window(std::string title, int width, int height);
		~Window();

		bool ShouldClose() const;
		void SwapBuffers() const;
		void PollEvents() const;

		GLFWwindow* const GetHandle() const;
	private:
		GLFWwindow* m_windowPtr;
	};
}
