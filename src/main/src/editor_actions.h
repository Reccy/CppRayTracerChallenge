#pragma once
#include "rogll/include.h"

struct EditorActions
{
	bool MoveDown = false;
	bool MoveUp = false;
	bool MoveLeft = false;
	bool MoveRight = false;
	bool MoveForward = false;
	bool MoveBackward = false;
	bool RotateXClockwise = false;
	bool RotateXCounterClockwise = false;
	bool RotateYClockwise = false;
	bool RotateYCounterClockwise = false;
	bool RotateZClockwise = false;
	bool RotateZCounterClockwise = false;
	bool PerformRender = false;
	bool UnselectObject = false;
	double MousePosX = 0;
	double MousePosY = 0;
	bool MouseLeftButtonDown = false;
	bool MouseLeftButtonHeld = false;
	bool MouseLeftButtonUp = false;
	float VMove = 0;
	float HMove = 0;
	float DMove = 0;
	float RotX = 0;
	float RotY = 0;
	float RotZ = 0;

	bool DecreaseFov = false;
	bool IncreaseFov = false;

	bool SelectPositionGizmo = false;
	bool SelectRotationGizmo = false;
	bool SelectScaleGizmo = false;

	bool ViewportCanCaptureMouse = false;
	bool ViewportCanCaptureKeyboard = false;

	void ReadInput(ROGLL::Window& window);
};
