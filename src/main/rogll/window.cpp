#include "window.h"
#include <iostream>
#include <sstream>

namespace ROGLL
{
	Window::Window(std::string title, int width, int height)
		: m_windowPtr(nullptr)
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		m_windowPtr = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

		if (m_windowPtr == nullptr)
		{
			std::cerr << "[ROGLL::Window] FATAL: Failed to create GLFW Window" << std::endl;
			abort();
		}

		glfwMakeContextCurrent(m_windowPtr);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cerr << "[ROGLL::Window] FATAL: Failed to initialize GLAD" << std::endl;
			abort();
		}

		glfwSwapInterval(1);
		glViewport(0, 0, width, height);
		glEnable(GL_DEPTH_TEST);

		const GLubyte* glVersion = glGetString(GL_VERSION);

		std::stringstream ss;
		ss << title << " (Driver: " << glVersion << ")";
		std::string titleNew = ss.str();

		glfwSetWindowTitle(m_windowPtr, titleNew.c_str());
		glfwSetWindowSizeLimits(m_windowPtr, 640, 480, GLFW_DONT_CARE, GLFW_DONT_CARE);

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();

		if (monitor == NULL)
		{
			std::cout << "[ROGLL::Window] WARNING: Could not find primary monitor. Failed to set window position." << std::endl;
		}
		else
		{
			const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);

			int centerX = vidmode->width * 0.5;
			int centerY = vidmode->height * 0.5;

			int sizeXHalf = width * 0.5;
			int sizeYHalf = height * 0.5;

			glfwSetWindowPos(m_windowPtr, centerX - sizeXHalf, centerY - sizeYHalf);
		}
	}

	Window::~Window()
	{
		glfwTerminate();
	}

	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(m_windowPtr);
	}

	void Window::SwapBuffers() const
	{
		glfwSwapBuffers(m_windowPtr);
	}

	void Window::PollEvents() const
	{
		glfwPollEvents();
	}

	GLFWwindow* const Window::GetHandle() const
	{
		return m_windowPtr;
	}
}
