#include "editor_actions.h"
#include <imgui.cpp>

void EditorActions::ReadInput(ROGLL::Window& windowRef)
{
	GLFWwindow* window = windowRef.GetHandle();

	ViewportCanCaptureKeyboard = !GImGui->IO.WantCaptureKeyboard;
	ViewportCanCaptureMouse = !GImGui->IO.WantCaptureMouse;

	MoveDown = glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS;
	MoveUp = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;
	MoveLeft = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
	MoveRight = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
	MoveForward = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
	MoveBackward = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;

	RotateXClockwise = glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS;
	RotateXCounterClockwise = glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS;
	RotateYClockwise = glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS;
	RotateYCounterClockwise = glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS;
	RotateZClockwise = glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS;
	RotateZCounterClockwise = glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS;

	PerformRender = glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS;
	UnselectObject = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;

	bool prevMouseLeftButtonHeld = MouseLeftButtonHeld;
	MouseLeftButtonHeld = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	MouseLeftButtonDown = MouseLeftButtonHeld && !prevMouseLeftButtonHeld;
	MouseLeftButtonUp = !MouseLeftButtonHeld && prevMouseLeftButtonHeld;

	SelectPositionGizmo = glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS;
	SelectRotationGizmo = glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS;
	SelectScaleGizmo = glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS;

	glfwGetCursorPos(window, &MousePosX, &MousePosY);

	VMove = (MoveUp * 1) - (MoveDown * 1);
	HMove = (MoveRight * 1) - (MoveLeft * 1);
	DMove = (MoveForward * 1) - (MoveBackward * 1);

	RotX = (RotateXClockwise * 1) - (RotateXCounterClockwise * 1);
	RotY = (RotateYClockwise * 1) - (RotateYCounterClockwise * 1);
	RotZ = (RotateZClockwise * 1) - (RotateZCounterClockwise * 1);

	IncreaseFov = glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS;
	DecreaseFov = glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS;
}
