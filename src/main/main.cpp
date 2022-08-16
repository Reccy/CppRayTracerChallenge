#include <iostream>
#include <sstream>

#include "glad.h"
#include "glfw3.h"
#include <RPly.h>

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

static int _PlyVertexCb(p_ply_argument argument)
{
	long attribute;
	std::vector<float>* vertexBuffer;
	ply_get_argument_user_data(argument, (void**)&vertexBuffer, &attribute);

	double data = ply_get_argument_value(argument);

	vertexBuffer->push_back(data);

	return 1;
}

static int faceParseIdx = 0;
static int _PlyFaceCb(p_ply_argument argument)
{
	if (faceParseIdx % 4 == 0)
	{
		faceParseIdx++;
		return 1;
	}

	faceParseIdx++;

	std::vector<unsigned int>* indexBuffer;
	ply_get_argument_user_data(argument, (void**)&indexBuffer, NULL);
	double data = ply_get_argument_value(argument);

	indexBuffer->push_back(data);

	return 1;
}

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

void _Draw(const ROGLL::VertexArray& vertexArray, const ROGLL::VertexBuffer& vertexBuffer, const ROGLL::IndexBuffer& indexBuffer, const ROGLL::Material& material)
{
	vertexArray.Bind();
	vertexBuffer.Bind();
	indexBuffer.Bind();
	material.Bind();

	glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, (void*)0);

	material.Unbind();
	indexBuffer.Unbind();
	vertexBuffer.Unbind();
	vertexArray.Unbind();
}

struct VertexLayout
{
	RML::Tuple3<float> pos;
	RML::Tuple3<float> normal;
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

	RML::Tuple3<float> Up(0, 1, 0);
	RML::Tuple3<float> Down(0, -1, 0);
	RML::Tuple3<float> Left(-1, 0, 0);
	RML::Tuple3<float> Right(1, 0, 0);
	RML::Tuple3<float> Forward(0, 0, 1);
	RML::Tuple3<float> Backward(0, 0, -1);

	RML::Tuple3<float> UP(2.0, 2.0, 2.0);

	ROGLL::VertexAttributes layout;
	layout.Add<float>(ROGLL::VertexAttributes::POSITION, 3);
	layout.Add<float>(ROGLL::VertexAttributes::NORMAL, 3);

	ROGLL::VertexAttributes gizmoLayout;
	gizmoLayout.Add<float>(ROGLL::VertexAttributes::POSITION, 3);
	gizmoLayout.Add<float>(ROGLL::VertexAttributes::COLOR, 3);

	VertexLayout cubeVertsLayout[]
	{
		// FRONT
		{ xyz, Backward },
		{ Xyz, Backward },
		{ XYz, Backward },
		{ xYz, Backward },
		//BACK
		{ xYZ, Forward }, // idx 4
		{ XYZ, Forward },
		{ XyZ, Forward },
		{ xyZ, Forward },
		//TOP
		{ xYz, Up }, // idx 8
		{ XYz, Up },
		{ XYZ, Up },
		{ xYZ, Up },
		//BOTTOM
		{ xyZ, Down }, // idx 12
		{ XyZ, Down },
		{ Xyz, Down },
		{ xyz, Down },
		//LEFT
		{ xyZ, Left }, // idx 16
		{ xyz, Left },
		{ xYz, Left },
		{ xYZ, Left },
		//RIGHT
		{ XyZ, Right }, // idx 20
		{ XYZ, Right },
		{ XYz, Right },
		{ Xyz, Right },
	};

	std::vector<float> cubeVertsFloats;

	for (const auto& vertex : cubeVertsLayout)
	{
		cubeVertsFloats.push_back(vertex.pos.x());
		cubeVertsFloats.push_back(vertex.pos.y());
		cubeVertsFloats.push_back(vertex.pos.z());
		cubeVertsFloats.push_back(vertex.normal.x());
		cubeVertsFloats.push_back(vertex.normal.y());
		cubeVertsFloats.push_back(vertex.normal.z());
	}

	ROGLL::Mesh cubeMesh(
		cubeVertsFloats,
		{
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
		},
		layout
	);

	ROGLL::MeshInstance cubeA(cubeMesh);
	ROGLL::MeshInstance cubeB(cubeMesh);
	cubeB.transform.translate(3, 3, 3);
	cubeB.transform.scale(1.5, 2, 2.5);

	ROGLL::MeshInstance lightCube(cubeMesh);
	lightCube.transform.scaling = { 0.5, 0.5, 0.5 };
	lightCube.transform.position = { 0, 10, 0 };

	std::vector<float> groundVertFloats
	{
		-100, -0.5, -100, 0, 1, 0,
		 100, -0.5, -100, 0, 1, 0,
		 100, -0.5,  100, 0, 1, 0,
		-100, -0.5,  100, 0, 1, 0
	};

	ROGLL::Mesh groundMesh(
		groundVertFloats,
		{
			0, 1, 2,
			2, 3, 0,
		},
		layout
	);

	ROGLL::MeshInstance groundMeshInstance(groundMesh);
	groundMeshInstance.transform.scale(100, 1, 100);

	// BEGIN GIZMO LOAD
	std::cout << "Loading gizmo3d file... ";

	p_ply gizmoPlyFile = ply_open("res/models/gizmo3d.ply", NULL, 0, NULL);
	if (!gizmoPlyFile || !ply_read_header(gizmoPlyFile))
	{
		std::cout << "ERROR: Could not read gizmo3d file" << std::endl;
		glfwTerminate();
		std::cin.get();
		return -1;
	}

	std::vector<float> gizmoPositions;
	std::vector<float> gizmoNormals;
	std::vector<float> gizmoColors;
	std::vector<unsigned int> gizmoIndices;

	long nvertices, ntriangles;
	nvertices = ply_set_read_cb(gizmoPlyFile, "vertex", "x", _PlyVertexCb, &gizmoPositions, 0);
	ply_set_read_cb(gizmoPlyFile, "vertex", "y", _PlyVertexCb, &gizmoPositions, 1);
	ply_set_read_cb(gizmoPlyFile, "vertex", "z", _PlyVertexCb, &gizmoPositions, 2);
	ply_set_read_cb(gizmoPlyFile, "vertex", "nx", _PlyVertexCb, &gizmoNormals, 3);
	ply_set_read_cb(gizmoPlyFile, "vertex", "ny", _PlyVertexCb, &gizmoNormals, 4);
	ply_set_read_cb(gizmoPlyFile, "vertex", "nz", _PlyVertexCb, &gizmoNormals, 5);
	ply_set_read_cb(gizmoPlyFile, "vertex", "red", _PlyVertexCb, &gizmoColors, 6);
	ply_set_read_cb(gizmoPlyFile, "vertex", "green", _PlyVertexCb, &gizmoColors, 7);
	ply_set_read_cb(gizmoPlyFile, "vertex", "blue", _PlyVertexCb, &gizmoColors, 8);
	
	ntriangles = ply_set_read_cb(gizmoPlyFile, "face", "vertex_indices", _PlyFaceCb, &gizmoIndices, NULL);
	
	if (!ply_read(gizmoPlyFile))
	{
		std::cout << "ERROR: Could not read gizmo3d file" << std::endl;
		glfwTerminate();
		std::cin.get();
		return -1;
	}

	ply_close(gizmoPlyFile);

	std::cout << "Successfully loaded gizmo3d file" << std::endl;
	// END GIZMO LOAD

	std::vector<float> gizmoVerts;

	for (long i = 0; i < nvertices; i++)
	{
		float offset = i * 3;
		gizmoVerts.push_back(gizmoPositions.at(offset));
		gizmoVerts.push_back(gizmoPositions.at(offset + 1));
		gizmoVerts.push_back(gizmoPositions.at(offset + 2));

		gizmoVerts.push_back(gizmoColors.at(offset) / 255);
		gizmoVerts.push_back(gizmoColors.at(offset + 1) / 255);
		gizmoVerts.push_back(gizmoColors.at(offset + 2) / 255);
	}

	ROGLL::Mesh gizmoMesh(gizmoVerts, gizmoIndices, gizmoLayout);

	ROGLL::MeshInstance gizmoMeshInstance(gizmoMesh);
	gizmoMeshInstance.transform.translate(0, 10, 0);
	gizmoMeshInstance.transform.scale(0.1, 0.1, 0.1);

	ROGLL::Shader shader("res/shaders/Default.shader");
	ROGLL::Shader gizmoShader("res/shaders/VertexColor.shader");

	ROGLL::Material gizmoMaterial(gizmoShader);
	gizmoMaterial.Set4("objectColor", White);

	ROGLL::Material cubeMaterial(shader);
	cubeMaterial.Set4("objectColor", Blue);

	ROGLL::Material groundMaterial(shader);
	groundMaterial.Set4("objectColor", Green);

	ROGLL::Material lightCubeMaterial(shader);
	lightCubeMaterial.Set4("objectColor", White);

	ROGLL::RenderBatch cubeBatch(&layout, &cubeMaterial);
	cubeBatch.AddInstance(&cubeA);
	cubeBatch.AddInstance(&cubeB);

	ROGLL::RenderBatch gizmoBatch(&gizmoLayout, &gizmoMaterial);
	gizmoBatch.AddInstance(&gizmoMeshInstance);

	ROGLL::RenderBatch groundBatch(&layout, &groundMaterial);
	groundBatch.AddInstance(&groundMeshInstance);

	ROGLL::RenderBatch lightCubeBatch(&layout, &lightCubeMaterial);
	lightCubeBatch.AddInstance(&lightCube);

	RML::Matrix<double, 4, 4> vp;
	RML::Matrix<double, 4, 4> mvp;

	RML::Transform model;
	ROGLL::Camera cam(WIDTH, HEIGHT, 60);
	cam.transform.translate(0, 0, -10); // Initial cam position

	RML::Tuple3<float> lightPosition {
		1,
		9,
		0
	};

	while (!window.ShouldClose())
	{
		_ProcessInput(window);
		_UpdateCamera(cam);
		
		cubeA.transform.rotate(0, 0.2, 0);
		cubeB.transform.rotate(0.2, 0.3, 0.5);

		glClearColor(ClearColor->x(), ClearColor->y(), ClearColor->z(), ClearColor->w());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cubeBatch.Render(cam, lightPosition);
		groundBatch.Render(cam, lightPosition);
		//lightCubeBatch.Render(cam, lightPosition);
		gizmoBatch.Render(cam, lightPosition);

		window.SwapBuffers();
		window.PollEvents();
	}

	return 0;
}
