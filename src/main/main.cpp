#include <iostream>
#include <sstream>

#include "glad.h"
#include "glfw3.h"

#include "rogll/include.h"
#include <RML.h>

// === Learning Resources ===
// Learn OpenGL: https://learnopengl.com/
// The Cherno's OpenGL Playlist: https://youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2
// Documentation: https://docs.gl/
// Open.GL: https://open.gl/

static RML::Tuple4<float> Red{ 1.0f, 0.0f, 0.0f, 1.0f };
static RML::Tuple4<float> Green{ 0.0f, 1.0f, 0.0f, 1.0f };
static RML::Tuple4<float> Blue{ 0.0f, 0.0f, 1.0f, 1.0f };
static RML::Tuple4<float> White{ 1.0f, 1.0f, 1.0f, 1.0f };
static RML::Tuple4<float> Black{ 0.0f, 0.0f, 0.0f, 0.0f };

static RML::Tuple4<float>* ClearColor = &Black;

static bool MoveDown = false;
static bool MoveUp = false;
static bool MoveLeft = false;
static bool MoveRight = false;
static bool MoveForward = false;
static bool MoveBackward = false;
static bool RotateXClockwise = false;
static bool RotateXCounterClockwise = false;
static bool RotateYClockwise = false;
static bool RotateYCounterClockwise = false;
static bool RotateZClockwise = false;
static bool RotateZCounterClockwise = false;
static float VMove = 0;
static float HMove = 0;
static float DMove = 0;
static float RotX = 0;
static float RotY = 0;
static float RotZ = 0;
static float Fov = 90;

static float CamXRot = 0;
static float CamYRot = 0;

static int WIDTH = 1024;
static int HEIGHT = 768;

static void _ProcessInput(const ROGLL::Window& windowRef)
{
	GLFWwindow* window = windowRef.GetHandle();

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

	VMove = (MoveUp * 1) - (MoveDown * 1);
	HMove = (MoveRight * 1) - (MoveLeft * 1);
	DMove = (MoveForward * 1) - (MoveBackward * 1);

	RotX = (RotateXClockwise * 1) - (RotateXCounterClockwise * 1);
	RotY = (RotateYClockwise * 1) - (RotateYCounterClockwise * 1);
	RotZ = (RotateZClockwise * 1) - (RotateZCounterClockwise * 1);

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		Fov += 0.5;

	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		Fov -= 0.5;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void _UpdateCamera(ROGLL::Camera& cam) {
	constexpr double speedFactor = 0.25;

	cam.SetPerspective(WIDTH, HEIGHT, RML::Trig::degrees_to_radians(Fov));
	cam.transform.position += cam.transform.rotation.inverse()
		* RML::Vector(HMove * speedFactor,
			VMove * speedFactor,
			DMove * speedFactor);

	CamXRot += RotX;
	CamYRot += RotY;

	cam.transform.rotation = RML::Quaternion::euler_angles(CamXRot, CamYRot, 0);
}

void _Draw(const ROGLL::VertexArray& vertexArray, const ROGLL::IndexBuffer& indexBuffer, const ROGLL::Material& material)
{
	vertexArray.Bind();
	indexBuffer.Bind();
	material.Bind();

	glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, (void*)0);

	material.Unbind();
	indexBuffer.Unbind();
	vertexArray.Unbind();
}

struct VertexLayout
{
	RML::Tuple3<float> pos;
};

int main(void)
{
	ROGLL::Window window("OpenGL Sandbox", WIDTH, HEIGHT);

	RML::Tuple3<float> xyz(-0.5, -0.5, -0.5);
	RML::Tuple3<float> xYz(-0.5, 0.5, -0.5);
	RML::Tuple3<float> Xyz(0.5, -0.5, -0.5);
	RML::Tuple3<float> XYz(0.5, 0.5, -0.5);

	RML::Tuple3<float> xyZ(-0.5, -0.5, 0.5);
	RML::Tuple3<float> xYZ(-0.5, 0.5, 0.5);
	RML::Tuple3<float> XyZ(0.5, -0.5, 0.5);
	RML::Tuple3<float> XYZ(0.5, 0.5, 0.5);

	VertexLayout cubeVertices[] = {
		// FRONT
		{ xyz },
		{ Xyz },
		{ XYz },
		{ xYz },
		//BACK
		{ xYZ }, // idx 4
		{ XYZ },
		{ XyZ },
		{ xyZ },
		//TOP
		{ xYz }, // idx 8
		{ XYz },
		{ XYZ },
		{ xYZ },
		//BOTTOM
		{ xyZ }, // idx 12
		{ XyZ },
		{ Xyz },
		{ xyz },
		//LEFT
		{ xyZ }, // idx 16
		{ xyz },
		{ xYz },
		{ xYZ },
		//RIGHT
		{ XyZ, }, // idx 20
		{ XYZ },
		{ XYz },
		{ Xyz },
	};

	VertexLayout groundVertices[] = {
		{ { -100, -0.5, -100 } },
		{ {  100, -0.5, -100 } },
		{ {  100, -0.5,  100 } },
		{ { -100, -0.5,  100 } },
	};

	unsigned int cubeIndices[] = {
		// FRONT
		0, 1, 2,
		2, 3, 0,
		// BACK
		4, 5, 6,
		6, 7, 4,
		// TOP
		8, 9, 10,
		10, 11, 8,
		// BOTTOM
		12, 13, 14,
		14, 15, 12,
		// LEFT
		16, 17, 18,
		18, 19, 16,
		// RIGHT
		20, 21, 22,
		22, 23, 20,
		// FLOOR
		24, 25, 26,
		26, 27, 24
	};

	unsigned int groundIndices[] = {
		0, 1, 2,
		2, 3, 0,
	};

	ROGLL::VertexArray cubeVertexArray;
	ROGLL::IndexBuffer cubeIndexLayout(cubeIndices, sizeof(cubeIndices));
	ROGLL::VertexBuffer cubeVertexBuffer(cubeVertices, sizeof(cubeVertices));

	ROGLL::VertexAttributes layout;
	layout.Add<float>(3); // XYZ Position

	cubeVertexArray.SetBuffer(layout, cubeVertexBuffer);

	ROGLL::VertexArray groundVertexArray;
	ROGLL::IndexBuffer groundIndexLayout(groundIndices, sizeof(groundIndices));
	ROGLL::VertexBuffer groundVertexBuffer(groundVertices, sizeof(groundVertices));

	groundVertexArray.SetBuffer(layout, groundVertexBuffer);

	ROGLL::Shader shader("res/shaders/Default.shader");

	ROGLL::Material cubeMaterial(shader);
	cubeMaterial.Set4("uColor", Red);

	ROGLL::Material groundMaterial(shader);
	groundMaterial.Set4("uColor", Green);

	RML::Matrix<double, 4, 4> vp;
	RML::Matrix<double, 4, 4> mvp;

	RML::Transform model;
	ROGLL::Camera cam(WIDTH, HEIGHT, 60);
	cam.transform.translate(0, 0, -10); // Initial cam position

	while (!window.ShouldClose())
	{
		_ProcessInput(window);
		_UpdateCamera(cam);
		
		vp = cam.GetProjectionMatrix() * cam.GetViewMatrix();
		mvp = vp * model.matrix();
		cubeMaterial.Set4x4("uMVP", mvp);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		_Draw(cubeVertexArray, cubeIndexLayout, cubeMaterial);
		_Draw(groundVertexArray, groundIndexLayout, groundMaterial);

		window.SwapBuffers();
		window.PollEvents();
	}

	return 0;
}
