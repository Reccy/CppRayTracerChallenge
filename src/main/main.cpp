#include <iostream>
#include <sstream>
#include <thread>
#include <algorithm>
#include <random>
#include <map>
#include <filesystem>

#define NOMINMAX
#include <windows.h>
#include <ShlObj.h>
#include <tchar.h>

#include <nfd.h>

#include "glad.h"
#include "glfw3.h"

#include <tinyxml2.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_internal.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include "src/rogll/include.h"
#include "src/editor_core.h"
#include "src/editor_io.h"
#include "src/editor_material.h"
#include "src/editor_object.h"
#include "src/editor_db.h"
#include "src/editor_pattern.h"
#include "src/editor_camera_settings.h"
#include "src/editor_actions.h"
#include "src/editor_gizmo.h"
#include "src/editor_axis.h"
#include "src/ply.h"

#include <RML.h>

#undef DIFFERENCE
#include <graphics/image.h>
#include <serializer/portable_network_graphics_serializer.h>
#include <math/comparison.h>
#include <renderer/camera.h>
#include <renderer/patterns/checker.h>
#include <renderer/patterns/stripe.h>
#include <math/plane.h>
#include <math/cube.h>
#include <math/plane.h>
#include <math/torus.h>
#include <math/cylinder.h>
#include <helpers/material_helper.h>

using namespace CppRayTracerChallenge::Core;

std::stringstream DebugStringStream;

static RML::Tuple4<float> Red{ 1.0f, 0.0f, 0.0f, 1.0f };
static RML::Tuple4<float> Green{ 0.0f, 1.0f, 0.0f, 1.0f };
static RML::Tuple4<float> Blue{ 0.0f, 0.0f, 1.0f, 1.0f };
static RML::Tuple4<float> White{ 1.0f, 1.0f, 1.0f, 1.0f };
static RML::Tuple4<float> Black{ 0.0f, 0.0f, 0.0f, 0.0f };

static RML::Tuple4<float>* ClearColor = &Black;

static Graphics::Color LightColor = Graphics::Color::white();

static RML::Tuple3<float> ColorToTuple(const Graphics::Color& c)
{
	return RML::Tuple3<float> { c.red(), c.green(), c.blue() };
}

static Graphics::Color TupleToColor(const RML::Tuple3<float>& tuple)
{
	return Graphics::Color(tuple.x(), tuple.y(), tuple.z());
}

static EditorActions Actions;

static CameraSettings CAMERA_SETTINGS { 90, 0, 0, 1024, 768 };

static int WINDOW_WIDTH = 1024;
static int WINDOW_HEIGHT = 768;

static ROGLL::Camera* MainCamera;

static GizmoType CurrentGizmoType = GizmoType::POSITION;

static EditorMaterial* SelectedMaterial = nullptr;

static std::vector<EditorObject*> EditorObjects;
static std::vector<EditorMaterial*> EditorMaterials;

static Math::Cube SharedCube;
static Math::Plane SharedPlane;
static Math::Sphere SharedSphere;
static Math::Cylinder SharedCylinder(-1, 1, true);
static Math::Torus SharedTorus(0.1, 1.25);

static RML::Vector _VectorClearNearZero(const RML::Vector& v)
{
	return RML::Vector(
		Math::Comparison::equal(v.x(), 0.0) ? 0.0 : v.x(),
		Math::Comparison::equal(v.y(), 0.0) ? 0.0 : v.y(),
		Math::Comparison::equal(v.z(), 0.0) ? 0.0 : v.z()
	);
}

static RML::Vector _ProjectVector(RML::Vector p, RML::Vector dir)
{
	RML::Point a = -dir;
	RML::Point b = dir;

	RML::Vector ap = p - a;
	RML::Vector ab = b - a;

	return a + ab * (RML::Vector::dot(ap, ab) / RML::Vector::dot(ab, ab));
}

static std::string _GetTypeName(EditorObject::Type objectType)
{
	if (objectType == EditorObject::Type::CUBE) return "Cube";
	if (objectType == EditorObject::Type::LIGHT) return "Light";
	if (objectType == EditorObject::Type::PLANE) return "Plane";
	if (objectType == EditorObject::Type::SPHERE) return "Sphere";
	if (objectType == EditorObject::Type::CYLINDER) return "Cylinder";

	assert(false); // Should never return unknown

	return "Unknown";
}

Math::IShape* _GetMathShapeForEditorObject(const EditorObject& editorObject)
{
	Math::IShape* result = nullptr;
	RML::Transform transform = editorObject.transform;

	if (editorObject.objectType == EditorObject::Type::CUBE || editorObject.objectType == EditorObject::Type::LIGHT)
	{
		result = &SharedCube;
		transform.scale(0.5, 0.5, 0.5); // Scale unit cube to match cube mesh
	}
	else if (editorObject.objectType == EditorObject::Type::PLANE)
	{
		result = &SharedPlane;
	}
	else if (editorObject.objectType == EditorObject::Type::SPHERE)
	{
		result = &SharedSphere;
	}
	else if (editorObject.objectType == EditorObject::Type::CYLINDER)
	{
		result = &SharedCylinder;
		transform.scale(0.5, 0.5, 0.5); // Scale unit cylinder to match cylinder mesh
	}

	assert(result != nullptr);

	result->transform(transform.matrix());

	return result;
}

double _IntersectRayWithEditorObject(Math::Ray ray, const EditorObject& editorObject)
{
	Math::IShape* shape = _GetMathShapeForEditorObject(editorObject);
	auto i = shape->intersect(ray);

	auto hit = i.hit();

	if (!hit.has_value()) return RML::INF;

	DebugStringStream << "Hit Location: " << ray.origin() + ray.direction() * hit.value().t() << "\n";

	RML::Transform t;
	t.position = (ray.origin() + ray.direction() * hit.value().t());

	return hit.value().t();
}

static RML::Transform _GetPlaneTransform(Axis axis, RML::Vector observerViewDirToPlane, GizmoType currentGizmoType)
{
	Math::Plane xzPlane;
	RML::Transform xzTransform;

	Math::Plane xyPlane;
	RML::Transform xyTransform;
	xyTransform.rotate(-90, 0, 0);
	xyPlane.transform(xyPlane.transform().rotate(-90, 0, 0));

	Math::Plane yzPlane;
	RML::Transform yzTransform;
	yzTransform.rotate(-90, -90, 0);
	yzPlane.transform(yzPlane.transform().rotate(-90, -90, 0));

	if (currentGizmoType == GizmoType::ROTATION)
	{
		if (axis == Axis::X)
		{
			return yzTransform;
		}
		else if (axis == Axis::Y)
		{
			return xzTransform;
		}
		else if (axis == Axis::Z)
		{
			return xyTransform;
		}
		else
		{
			assert(false);
		}
	}

	RML::Transform result;
	double xyDot = abs(RML::Vector::dot(xyPlane.normal(RML::Point(0, 0, 0)), observerViewDirToPlane));
	double xzDot = abs(RML::Vector::dot(xzPlane.normal(RML::Point(0, 0, 0)), observerViewDirToPlane));
	double yzDot = abs(RML::Vector::dot(yzPlane.normal(RML::Point(0, 0, 0)), observerViewDirToPlane));

	if (axis == Axis::X)
	{
		if (xyDot > xzDot)
		{
			return xyTransform;
		}
		else
		{
			return xzTransform;
		}
	}
	else if (axis == Axis::Y)
	{
		if (xyDot > yzDot)
		{
			return xyTransform;
		}
		else
		{
			return yzTransform;
		}
	}
	else if (axis == Axis::Z)
	{
		if (xzDot > yzDot)
		{
			return xzTransform;
		}
		else
		{
			return yzTransform;
		}
	}

	assert(false);
	
	return RML::Transform();
}

static RML::Vector _GetTransformDirectionByAxisAndObject(const Axis axis, const EditorObject& obj)
{
	if (axis == Axis::X) return obj.transform.right();

	if (axis == Axis::Y) return obj.transform.up();

	if (axis == Axis::Z) return obj.transform.forward();

	assert(false);

	return RML::Vector::forward();
}

std::string _GetAxisString(Axis axis)
{
	if (axis == Axis::NONE) return "NONE";
	if (axis == Axis::X) return "X";
	if (axis == Axis::Y) return "Y";
	if (axis == Axis::Z) return "Z";

	assert(false); // This should never hit

	return "NONE";
}

static Gizmo* PositionGizmo;
static Gizmo* RotationGizmo;
static Gizmo* ScaleGizmo;
static Gizmo** CurrentGizmoPtr = &PositionGizmo;

enum class ObjectPickerType
{
	NONE,
	EDITOR_OBJECT,
	GIZMO_HANDLE
};

struct ObjectPickerHit
{
	ObjectPickerType type = ObjectPickerType::NONE;
	Axis axis = Axis::NONE;
	RML::Vector worldPositionHit;
	Math::Ray ray = Math::Ray(RML::Point(), RML::Vector());
	void* ptr = nullptr;
};

static ObjectPickerHit _SelectObjectUnderCursor(ROGLL::Camera* camera, bool anObjectIsCurrentlySelected)
{
	ObjectPickerHit result;

	if (Actions.MousePosX < 0 || Actions.MousePosX > WINDOW_WIDTH || Actions.MousePosY < 0 || Actions.MousePosY > WINDOW_HEIGHT)
	{
		return result;
	}

	Math::Ray ray = camera->RayForPixel(Actions.MousePosX, Actions.MousePosY, WINDOW_WIDTH, WINDOW_HEIGHT, CAMERA_SETTINGS.fov);
	result.ray = ray;

	if (anObjectIsCurrentlySelected)
	{
		auto& CurrentGizmo = *CurrentGizmoPtr;
		RML::Vector worldPositionHit;
		Axis hitAxis = CurrentGizmo->GetAxisForIntersectedRay(ray, worldPositionHit);

		if (hitAxis != Axis::NONE)
		{
			result.type = ObjectPickerType::GIZMO_HANDLE;
			result.worldPositionHit = worldPositionHit;
			result.axis = hitAxis;
			result.ptr = CurrentGizmo;
			return result;
		};
	}

	result.type = ObjectPickerType::EDITOR_OBJECT;
	double closestEditorObject = RML::INF;

	for (const auto& editorObject : EditorObjects)
	{
		double currentClosest = _IntersectRayWithEditorObject(ray, *editorObject);

		if (currentClosest < closestEditorObject)
		{
			closestEditorObject = currentClosest;
			result.worldPositionHit = ray.origin() + ray.direction() * currentClosest;
			result.ptr = editorObject;
		}
	}

	return result;
}

IMGUI_API bool _DearImGui_BeginStatusBar()
{
	using namespace ImGui;

	ImGuiContext& g = *GImGui;
	ImGuiViewportP* viewport = (ImGuiViewportP*)(void*)GetMainViewport();

	// Notify of viewport change so GetFrameHeight() can be accurate in case of DPI change
	SetCurrentViewport(NULL, viewport);

	// For the main menu bar, which cannot be moved, we honor g.Style.DisplaySafeAreaPadding to ensure text can be visible on a TV set.
	// FIXME: This could be generalized as an opt-in way to clamp window->DC.CursorStartPos to avoid SafeArea?
	// FIXME: Consider removing support for safe area down the line... it's messy. Nowadays consoles have support for TV calibration in OS settings.
	g.NextWindowData.MenuBarOffsetMinVal = ImVec2(g.Style.DisplaySafeAreaPadding.x, ImMax(g.Style.DisplaySafeAreaPadding.y - g.Style.FramePadding.y, 0.0f));
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
	float height = GetFrameHeight();
	bool is_open = BeginViewportSideBar("##StatusMenuBar", viewport, ImGuiDir_Down, height, window_flags);
	g.NextWindowData.MenuBarOffsetMinVal = ImVec2(0.0f, 0.0f);

	if (is_open)
		BeginMenuBar();
	else
		End();
	return is_open;
}

void _DearImGui_EndStatusBar()
{
	using namespace ImGui;

	EndMenuBar();

	// When the user has left the menu layer (typically: closed menus through activation of an item), we restore focus to the previous window
	// FIXME: With this strategy we won't be able to restore a NULL focus.
	ImGuiContext& g = *GImGui;
	if (g.CurrentWindow == g.NavWindow && g.NavLayer == ImGuiNavLayer_Main && !g.NavAnyRequest)
		FocusTopMostWindowUnderOne(g.NavWindow, NULL);

	End();
}

void _DearImGui_SetNextWindowPosRelative(const ImVec2& vec)
{
	ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->Pos.x + vec.x, ImGui::GetMainViewport()->Pos.y + vec.y));
}

void _UpdateCamera(ROGLL::Camera& cam) {
	constexpr double speedFactor = 0.25;

	cam.SetPerspective(WINDOW_WIDTH, WINDOW_HEIGHT, RML::Trig::degrees_to_radians(CAMERA_SETTINGS.fov));
	cam.transform.position += cam.transform.rotation.inverse()
		* RML::Vector(Actions.HMove * speedFactor,
			Actions.VMove * speedFactor,
			Actions.DMove * speedFactor);

	CAMERA_SETTINGS.xRot += Actions.RotX;
	CAMERA_SETTINGS.yRot += Actions.RotY;

	constexpr float camXRotLimit = 89;

	CAMERA_SETTINGS.xRot = std::clamp(CAMERA_SETTINGS.xRot, -camXRotLimit, camXRotLimit);

	cam.transform.rotation = RML::Quaternion::euler_angles(CAMERA_SETTINGS.xRot, CAMERA_SETTINGS.yRot, 0);
}

struct VertexLayout
{
	RML::Tuple3<float> pos;
	RML::Tuple3<float> normal;
};

struct UiTextureVertexLayout
{
	RML::Tuple3<float> pos;
	RML::Tuple2<float> uv;
};

static void _WindowResized(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;
}

static EditorMaterial* _CreateNewEditorMaterial()
{
	EditorMaterial* mat = new EditorMaterial();
	mat->id = EditorDB::GenerateUniqueID();
	mat->name = "New Material";
	EditorMaterials.push_back(mat);
	return EditorMaterials[EditorMaterials.size() - 1];
}

ROGLL::Mesh* CubeMesh;
ROGLL::Mesh* CylinderMesh;
ROGLL::Mesh* LightMesh;
ROGLL::Mesh* PlaneMesh;
ROGLL::Mesh* SphereMesh;

static EditorObject* _CreateLight(RML::Vector position)
{
	EditorObject* light = new EditorObject();
	light->id = EditorDB::GenerateUniqueID();
	light->name = "Light";
	light->transform.position = position;
	light->objectType = EditorObject::Type::LIGHT;
	light->meshInstance = EditorObject::CreateMeshInstanceForType(EditorObject::Type::LIGHT);
	EditorObjects.push_back(light);
	return EditorObjects[EditorObjects.size() - 1];
}

static EditorObject* _CreateCube(RML::Vector position)
{
	EditorObject* cube = new EditorObject();
	cube->id = EditorDB::GenerateUniqueID();
	cube->name = "Cube";
	cube->transform.position = position;
	cube->objectType = EditorObject::Type::CUBE;
	cube->meshInstance = EditorObject::CreateMeshInstanceForType(EditorObject::Type::CUBE);
	EditorObjects.push_back(cube);
	return EditorObjects[EditorObjects.size() - 1];
}

static EditorObject* _CreatePlane(RML::Vector position)
{
	EditorObject* plane = new EditorObject();
	plane->id = EditorDB::GenerateUniqueID();
	plane->name = "Ground Plane";
	plane->transform.position = position;
	plane->objectType = EditorObject::Type::PLANE;
	plane->meshInstance = EditorObject::CreateMeshInstanceForType(EditorObject::Type::PLANE);
	EditorObjects.push_back(plane);
	return EditorObjects[EditorObjects.size() - 1];
}

static EditorObject* _CreateSphere(RML::Vector position)
{
	EditorObject* sphere = new EditorObject();
	sphere->id = EditorDB::GenerateUniqueID();
	sphere->name = "Sphere";
	sphere->transform.position = position;
	sphere->objectType = EditorObject::Type::SPHERE;
	sphere->meshInstance = EditorObject::CreateMeshInstanceForType(EditorObject::Type::SPHERE);
	EditorObjects.push_back(sphere);
	return EditorObjects[EditorObjects.size() - 1];
}

static EditorObject* _CreateCylinder(RML::Vector position)
{
	EditorObject* cylinder = new EditorObject();
	cylinder->id = EditorDB::GenerateUniqueID();
	cylinder->name = "Cylinder";
	cylinder->transform.position = position;
	cylinder->objectType = EditorObject::Type::CYLINDER;
	cylinder->meshInstance = EditorObject::CreateMeshInstanceForType(EditorObject::Type::CYLINDER);
	EditorObjects.push_back(cylinder);
	return EditorObjects[EditorObjects.size() - 1];
}

int main(void)
{
	ROGLL::Window window("Reccy's Ray Tracer", WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetWindowSizeCallback(window.GetHandle(), _WindowResized);
	// Open GL Initialized from here

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

	ROGLL::VertexAttributes gizmoLayout;
	gizmoLayout.Add<float>(ROGLL::VertexAttributes::POSITION3, 3);
	gizmoLayout.Add<float>(ROGLL::VertexAttributes::COLOR3, 3);

	ROGLL::VertexAttributes lightLayout;
	lightLayout.Add<float>(ROGLL::VertexAttributes::POSITION3, 3);
	lightLayout.Add<float>(ROGLL::VertexAttributes::COLOR3, 3);

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

	CubeMesh = new ROGLL::Mesh(
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
		EditorCore::GetDefaultLayout()
		);

	auto fM = 1000.0f;

	std::vector<float> planeVertFloats
	{
		-fM, 0, -fM, 0, 1, 0,
		 fM, 0, -fM, 0, 1, 0,
		 fM, 0,  fM, 0, 1, 0,
		-fM, 0,  fM, 0, 1, 0
	};

	PlaneMesh = new ROGLL::Mesh(
		planeVertFloats,
		{
			0, 1, 2,
			2, 3, 0,
		},
		EditorCore::GetDefaultLayout()
	);

	SphereMesh = PLY::LoadMesh("res/models/sphere.ply", EditorCore::GetDefaultLayout());
	CylinderMesh = PLY::LoadMesh("res/models/cylinder.ply", EditorCore::GetDefaultLayout());

	LightMesh = PLY::LoadMesh("res/models/light.ply", lightLayout);

	ROGLL::Mesh* gizmoPositionMesh = PLY::LoadMesh("res/models/gizmo3d_position.ply", gizmoLayout);
	ROGLL::Mesh* gizmoRotationMesh = PLY::LoadMesh("res/models/gizmo3D_rotation.ply", gizmoLayout);
	ROGLL::Mesh* gizmoScaleMesh = PLY::LoadMesh("res/models/gizmo3d_scale.ply", gizmoLayout);

	ROGLL::MeshInstance gizmoMeshInstance(*gizmoPositionMesh);
	ROGLL::MeshInstance positionHandleMeshInstance(*gizmoPositionMesh);
	ROGLL::MeshInstance rotationHandleMeshInstance(*gizmoRotationMesh);
	ROGLL::MeshInstance scaleHandleMeshInstance(*gizmoScaleMesh);

	double offsetFromOrigin = 0.73;
	double width = 0.1;
	double length = 0.73;

	PositionGizmo = new Gizmo(*gizmoPositionMesh,
		{ RML::Transform().translate(offsetFromOrigin, 0, 0).scale(length, width, width), &SharedCube},
		{ RML::Transform().translate(0, offsetFromOrigin, 0).scale(width, length, width), &SharedCube},
		{ RML::Transform().translate(0, 0, offsetFromOrigin).scale(width, width, length), &SharedCube});
	
	RotationGizmo = new Gizmo(*gizmoRotationMesh,
		{ RML::Transform().rotate(90, 90, 0), &SharedTorus},
		{ RML::Transform(), &SharedTorus},
		{ RML::Transform().rotate(90, 0, 0), &SharedTorus});

	ScaleGizmo = new Gizmo(*gizmoScaleMesh,
		{ RML::Transform().translate(offsetFromOrigin, 0, 0).scale(length, width, width), &SharedCube},
		{ RML::Transform().translate(0, offsetFromOrigin, 0).scale(width, length, width), &SharedCube},
		{ RML::Transform().translate(0, 0, offsetFromOrigin).scale(width, width, length), &SharedCube});

	RML::Tuple3<float> xy(-0.5, -0.5, 0);
	RML::Tuple3<float> Xy(0.5, -0.5, 0);
	RML::Tuple3<float> XY(0.5, 0.5, 0);
	RML::Tuple3<float> xY(-0.5, 0.5, 0);

	UiTextureVertexLayout uiTextureVerts[] = {
		xy, { 0, 0 },
		Xy, { 1, 0 },
		XY, { 1, 1 },
		xY, { 0, 1 }
	};

	std::vector<float> uiTextureFloats;

	for (const auto& vert : uiTextureVerts)
	{
		uiTextureFloats.push_back(vert.pos.x());
		uiTextureFloats.push_back(vert.pos.y());
		uiTextureFloats.push_back(vert.pos.z());

		uiTextureFloats.push_back(vert.uv.x());
		uiTextureFloats.push_back(vert.uv.y());
	}

	ROGLL::Shader gizmoShader("res/shaders/Gizmo.shader");
	ROGLL::Shader lightShader("res/shaders/Light.shader");

	ROGLL::Material gizmoMaterial(gizmoShader);
	gizmoMaterial.Set4("objectColor", White);
	gizmoMaterial.Set3("handleActive", RML::Tuple3<float>( 1, 1, 1 ));

	ROGLL::Material handleMaterial(gizmoShader);
	handleMaterial.Set4("objectColor", White);
	handleMaterial.Set3("handleActive", RML::Tuple3<float>( 0, 0, 0 ));

	ROGLL::Material lightMaterial(lightShader);
	lightMaterial.Set4("objectColor", RML::Tuple4<float>(0.1f, 0.2f, 0.1f, 1.0f));

	ROGLL::Material debugMeshMaterial(EditorCore::GetDefaultShader());
	debugMeshMaterial.Set4("objectColor", Red + Green);

	ROGLL::RenderBatch gizmoBatch(&gizmoLayout, &gizmoMaterial);
	gizmoBatch.AddInstance(&gizmoMeshInstance);

	ROGLL::MeshInstance* currentHandleMeshInstance = &positionHandleMeshInstance;

	ROGLL::RenderBatch handleBatch(&gizmoLayout, &handleMaterial);

	ROGLL::RenderBatch lightBatch(&lightLayout, &lightMaterial);

	ROGLL::Camera editorCamera(WINDOW_WIDTH, WINDOW_HEIGHT, 60);
	editorCamera.transform.translate(0, 0, -10); // Initial cam position

	MainCamera = &editorCamera;

	ROGLL::Camera gizmoCam(32, 32, 1); // Psuedo orthograpic projection
	gizmoCam.transform.translate(0, 0, -3.2);

	RML::Tuple3<float> lightPosition{
		1,
		9,
		0
	};

	// IMGUI BEGIN
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	std::filesystem::path file{ "imgui.ini" };
	if (!std::filesystem::exists(file))
	{
		std::cout << "Loading default layout file" << std::endl;
		ImGui::LoadIniSettingsFromDisk("imgui_default_layout.ini");
	}
	else
	{
		std::cout << "Loading saved layout file" << std::endl;
	}

	std::cout << io.IniFilename << std::endl;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window.GetHandle(), true);
	ImGui_ImplOpenGL3_Init("#version 330 core");

	const ImGuiViewport* const imguiViewport = ImGui::GetMainViewport();
	// IMGUI END

	// New Render Target Begin (For Gizmo)
	GLuint gizmoFramebufferId;
	glGenFramebuffers(1, &gizmoFramebufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, gizmoFramebufferId);

	GLuint gizmoRenderTextureId;
	glGenTextures(1, &gizmoRenderTextureId);
	glBindTexture(GL_TEXTURE_2D, gizmoRenderTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	GLuint gizmoRenderBufferId;
	glGenRenderbuffers(1, &gizmoRenderBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, gizmoRenderBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, gizmoRenderBufferId);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, gizmoRenderTextureId, 0);

	GLenum gizmoDrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, gizmoDrawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR: Could not create framebuffer" << std::endl;
		glfwTerminate();
		std::cin.get();
		return -1;
	}

	// New Render Target End

	// New Render Target Begin (For Render)
	GLuint raytracerRenderTextureId;
	glGenTextures(1, &raytracerRenderTextureId);
	glBindTexture(GL_TEXTURE_2D, raytracerRenderTextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	auto light = _CreateLight({ 0, 0, 0 });

	// New Render Target End

	bool guiShowDearImGuiDemo = false;
	bool guiShowGizmo = true;
	bool guiShowMaterials = false;
	bool guiIsRendering = false;
	bool guiShowRenderSettings = false;
	bool guiShowRenderPreview = false;
	bool guiShowDevDebugConsole = false;
	
	EditorObject* selectedObject = nullptr;
	Axis selectedObjectHitAxis = Axis::NONE;

	RML::Vector selectedObjectScalingStart;
	RML::Vector selectedObjectAngleAxisRotationStart;
	RML::Quaternion selectedObjectQuaternionRotationStart = RML::Quaternion::euler_angles(1,0,0);
	RML::Vector dirSelectedObjectToMouseDragWorldPosStart;
	RML::Vector selectedObjectHitWorldPosStart;
	RML::Vector selectedObjectPositionStart;
	Math::Plane selectedObjectGizmoAxisPlane;
	RML::Transform selectedObjectGizmoAxisPlaneTransform;

	while (!window.ShouldClose())
	{
		Actions.ReadInput(window);

		if (Actions.SelectPositionGizmo)
		{
			CurrentGizmoType = GizmoType::POSITION;
			CurrentGizmoPtr = &PositionGizmo;
		}
		else if (Actions.SelectRotationGizmo)
		{
			CurrentGizmoType = GizmoType::ROTATION;
			CurrentGizmoPtr = &RotationGizmo;
		}
		else if (Actions.SelectScaleGizmo)
		{
			CurrentGizmoType = GizmoType::SCALE;
			CurrentGizmoPtr = &ScaleGizmo;
		}

		if (Actions.IncreaseFov)
		{
			CAMERA_SETTINGS.fov += 0.5;
		}

		if (Actions.DecreaseFov)
		{
			CAMERA_SETTINGS.fov -= 0.5;
		}

		CAMERA_SETTINGS.fov = std::clamp(CAMERA_SETTINGS.fov, 5.0f, 170.0f);

		auto lightColorTuple = ColorToTuple(LightColor);

		// Light cannot be rotated or scaled - force user to use the POSITION Gizmo
		if (selectedObject != nullptr && selectedObject->objectType == EditorObject::Type::LIGHT)
		{
			CurrentGizmoType = GizmoType::POSITION;
			CurrentGizmoPtr = &PositionGizmo;
		}

		if (CurrentGizmoType == GizmoType::POSITION)
		{
			currentHandleMeshInstance = &positionHandleMeshInstance;
		}
		else if (CurrentGizmoType == GizmoType::ROTATION)
		{
			currentHandleMeshInstance = &rotationHandleMeshInstance;
		}
		else if (CurrentGizmoType == GizmoType::SCALE)
		{
			currentHandleMeshInstance = &scaleHandleMeshInstance;
		}

		handleBatch.AddInstance(currentHandleMeshInstance);

		if (Actions.PerformRender && !EditorCore::IsRenderInProgress())
		{
			EditorCore::StartFullRender(CAMERA_SETTINGS.renderWidth, CAMERA_SETTINGS.renderHeight, CAMERA_SETTINGS.fov, editorCamera, EditorObjects, LightColor);
		}

		ObjectPickerHit selectedObjectHit;
		
		if (Actions.ViewportCanCaptureMouse)
		{
			selectedObjectHit = _SelectObjectUnderCursor(MainCamera, selectedObject != nullptr);
		}

		if (selectedObjectHit.type == ObjectPickerType::GIZMO_HANDLE)
		{
			if (selectedObjectHit.axis == Axis::X)
			{
				handleMaterial.Set3("handleActive", RML::Tuple3<float>(1, 0, 0));
			}
			else if (selectedObjectHit.axis == Axis::Y)
			{
				handleMaterial.Set3("handleActive", RML::Tuple3<float>(0, 1, 0));
			}
			else if (selectedObjectHit.axis == Axis::Z)
			{
				handleMaterial.Set3("handleActive", RML::Tuple3<float>(0, 0, 1));
			}
			else
			{
				assert(false); // this should not be hit
			}
		}
		else
		{
			handleMaterial.Set3("handleActive", RML::Tuple3<float>(0, 0, 0));
		}

		if (Actions.ViewportCanCaptureMouse && Actions.MouseLeftButtonDown)
		{
			if (selectedObjectHit.type == ObjectPickerType::EDITOR_OBJECT)
			{
				selectedObject = static_cast<EditorObject*>(selectedObjectHit.ptr);
			}
			else if (selectedObjectHit.type == ObjectPickerType::GIZMO_HANDLE)
			{
				selectedObjectHitAxis = selectedObjectHit.axis;
				selectedObjectHitWorldPosStart = selectedObjectHit.worldPositionHit;
				selectedObjectPositionStart = selectedObject->transform.position;
				selectedObjectScalingStart = selectedObject->transform.scaling;
				selectedObjectQuaternionRotationStart = selectedObject->transform.rotation;
				dirSelectedObjectToMouseDragWorldPosStart = RML::Vector(selectedObjectHitWorldPosStart - selectedObject->transform.position).normalize();
				selectedObjectGizmoAxisPlane = Math::Plane();
				selectedObjectGizmoAxisPlaneTransform = _GetPlaneTransform(selectedObjectHitAxis, selectedObjectHit.ray.direction(), CurrentGizmoType);
				selectedObjectGizmoAxisPlaneTransform.translate(selectedObjectHitWorldPosStart.x(), selectedObjectHitWorldPosStart.y(), selectedObjectHitWorldPosStart.z());
			}
			else
			{
				selectedObject = nullptr;
			}
		}

		if (selectedObjectHitAxis != Axis::NONE)
		{
			RML::Transform axisPlaneTransform = selectedObjectGizmoAxisPlaneTransform;
			const RML::Vector axisPlaneNormal = axisPlaneTransform.up();

			axisPlaneTransform.rotation = axisPlaneTransform.rotation * selectedObject->transform.rotation;
			Math::Plane axisPlane = selectedObjectGizmoAxisPlane;
			const RML::Vector axisPlaneInputNormal = axisPlaneTransform.down();

			axisPlane.transform(axisPlaneTransform.matrix());

			const Math::Ray ray = MainCamera->RayForPixel(Actions.MousePosX, Actions.MousePosY, WINDOW_WIDTH, WINDOW_HEIGHT, CAMERA_SETTINGS.fov);
			const auto intersections = axisPlane.intersect(ray);

			if (intersections.hit().has_value())
			{
				const auto hit = intersections.hit().value();
				const RML::Vector hitPosition = ray.origin() + ray.direction() * hit.t();
				
				RML::Vector mouseWorldPosDelta = hitPosition - selectedObjectHitWorldPosStart;
				mouseWorldPosDelta = _VectorClearNearZero(mouseWorldPosDelta);
				const RML::Vector gizmoPositiveAxisVector = _GetTransformDirectionByAxisAndObject(selectedObjectHitAxis, *selectedObject);
				const RML::Vector deltaAlongAxis = _ProjectVector(mouseWorldPosDelta, gizmoPositiveAxisVector);

				if (CurrentGizmoType == GizmoType::POSITION)
				{
					selectedObject->transform.position = selectedObjectPositionStart + deltaAlongAxis;
				}
				else if (CurrentGizmoType == GizmoType::ROTATION)
				{
					RML::Vector currentDirOriginToHit = RML::Vector(hitPosition - selectedObject->transform.position).normalized();

					const double angle = RML::Vector::signed_angle(currentDirOriginToHit, dirSelectedObjectToMouseDragWorldPosStart, axisPlaneInputNormal);
					
					if (!std::isnan(angle))
					{
						auto deltaRotation = RML::Quaternion::angle_axis(angle, axisPlaneNormal);
						RML::Quaternion newRotation = deltaRotation * selectedObjectQuaternionRotationStart;
						selectedObject->transform.rotation = newRotation;
						selectedObject->eulerRotation = newRotation.to_euler();
					}
				}
				else if (CurrentGizmoType == GizmoType::SCALE)
				{
					const RML::Vector globalPositiveAxis = AxisToDirection(selectedObjectHitAxis);
					const RML::Vector localAxis = EditorObject::AxisToLocalDirection(*selectedObject, selectedObjectHitAxis);

					RML::Vector scalingVector = _ProjectVector(mouseWorldPosDelta, localAxis);

					const double deltaScaling = RML::Vector::dot(scalingVector, localAxis);

					RML::Vector delta = globalPositiveAxis * deltaScaling;

					selectedObject->transform.scaling = selectedObjectScalingStart + delta;
				}
				else
				{
					assert(false);
				}
			}
		}

		if (Actions.MouseLeftButtonUp)
		{
			selectedObjectHitAxis = Axis::NONE;
		}

		if (Actions.UnselectObject)
		{
			selectedObject = nullptr;
		}

		// Only update the camera if the user has not selected a handle axis
		// This is to prevent the camera moving during drag and breaking the cursor delta calculation
		// Also only update camera if user has focused on the viewport, not a UI element
		if (selectedObjectHitAxis == Axis::NONE && Actions.ViewportCanCaptureKeyboard)
		{
			_UpdateCamera(editorCamera);
		}

		// UI - Pre Pass
		glBindFramebuffer(GL_FRAMEBUFFER, gizmoFramebufferId);
		glViewport(0, 0, 512, 512);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		gizmoMeshInstance.transform.rotation = editorCamera.transform.rotation.inverse();
		gizmoBatch.Render(gizmoCam, lightPosition, lightColorTuple);

		if (EditorCore::IsRenderInProgress())
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CAMERA_SETTINGS.renderWidth, CAMERA_SETTINGS.renderHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, EditorCore::GetRenderPixels());
			glBindTexture(GL_TEXTURE_2D, raytracerRenderTextureId);
		}

		// Geometry Pass
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glClearColor(ClearColor->x(), ClearColor->y(), ClearColor->z(), ClearColor->w());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// Update the mesh instance transforms and set up render batches (since they are different to the editor object transform)
		for (const auto& objPtr : EditorObjects)
		{
			const auto& obj = *objPtr;

			if (obj.objectType == EditorObject::Type::LIGHT)
			{
				lightPosition = RML::Tuple3<float>(obj.transform.position.x(), obj.transform.position.y(), obj.transform.position.z());
			}

			obj.meshInstance->transform = obj.transform;

			if (obj.objectType == EditorObject::Type::LIGHT)
			{
				lightBatch.AddInstance(obj.meshInstance);
			}
			else
			{
				obj.material->renderBatch->AddInstance(obj.meshInstance);
			}
		}

		for (auto& material : EditorMaterials)
		{
			// TODO: Support multiple patterns
			Graphics::Color c = static_cast<EditorPatternSolidColor*>(material->editorPattern)->color;
			material->previewMaterial->Set4("objectColor", RML::Tuple4<float>(c.red(), c.green(), c.blue(), 1));
			material->renderBatch->Render(editorCamera, lightPosition, lightColorTuple);
		}

		lightBatch.Render(editorCamera, lightPosition, lightColorTuple);

		// Finished World 3D Render
		glClear(GL_DEPTH_BUFFER_BIT);

		auto& CurrentGizmo = *CurrentGizmoPtr;

		if (selectedObject != nullptr)
		{
			currentHandleMeshInstance->transform.position = selectedObject->transform.position;
			currentHandleMeshInstance->transform.rotation = selectedObject->transform.rotation;
			
			float dist = RML::Vector(selectedObject->transform.position - MainCamera->transform.position).magnitude();
			float scaling = dist * 0.2f;
			RML::Vector scalingVector(scaling, scaling, scaling);
			
			currentHandleMeshInstance->transform.scaling = scalingVector;
			CurrentGizmo->transform = selectedObject->transform;
			CurrentGizmo->transform.scaling = scalingVector;
			
			handleBatch.Render(editorCamera, lightPosition, lightColorTuple);
		}

		for (auto& material : EditorMaterials)
		{
			material->renderBatch->Clear();
		}

		lightBatch.Clear();
		handleBatch.Clear();

		//IMGUI TEST BEGIN

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

		// Render Main Menu Bar
		{
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Save as..."))
					{
						std::cout << "Begin Save..." << std::endl;

						nfdchar_t* outPath = NULL;
						nfdresult_t result = NFD_SaveDialog("rrt", NULL, &outPath); // .rrt - Reccy's Ray Tracer

						if (result == NFD_OKAY)
						{
							std::string path(outPath);
							EditorIO::SaveWorld(path, CAMERA_SETTINGS, editorCamera, EditorObjects, EditorMaterials, LightColor);
							free(outPath);
						}
						else if (result == NFD_CANCEL)
						{
							std::cout << "Save Cancelled" << std::endl;
						}
						else
						{
							std::cout << "Error: " << NFD_GetError() << std::endl;
						}
					}

					if (ImGui::MenuItem("Open"))
					{
						std::cout << "Begin Open..." << std::endl;

						nfdchar_t* outPath = NULL;
						nfdresult_t result = NFD_OpenDialog("rrt", NULL, &outPath); // .rrt - Reccy's Ray Tracer

						if (result == NFD_OKAY)
						{
							std::string path(outPath);

							// Unselect selected object and selected material since it might not exist in new world
							selectedObject = nullptr;
							SelectedMaterial = nullptr;
							EditorIO::OpenWorld(path, CAMERA_SETTINGS, editorCamera, EditorObjects, EditorMaterials, LightColor);
							free(outPath);
						}
						else if (result == NFD_CANCEL)
						{
							std::cout << "Open Cancelled" << std::endl;
						}
						else
						{
							std::cout << "Error: " << NFD_GetError() << std::endl;
						}
					}

					if (ImGui::MenuItem("Exit", "Alt + F4")) {
						glfwSetWindowShouldClose(window.GetHandle(), 1);
					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Add"))
				{
					if (ImGui::MenuItem("Cube")) {
						auto obj = _CreateCube({ 0,0,0 });
						obj->name = "New Cube";
						selectedObject = obj;
					}

					if (ImGui::MenuItem("Plane")) {
						auto obj = _CreatePlane({ 0, 0, 0 });
						obj->name = "New Plane";
						selectedObject = obj;
					}

					if (ImGui::MenuItem("Sphere")) {
						auto obj = _CreateSphere({ 0, 0, 0 });
						obj->name = "New Sphere";
						selectedObject = obj;
					}

					if (ImGui::MenuItem("Cylinder")) {
						auto obj = _CreateCylinder({ 0, 0, 0 });
						obj->name = "New Cylinder";
						selectedObject = obj;
					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Render"))
				{
					ImGui::MenuItem("Settings", nullptr, &guiShowRenderSettings);
					ImGui::MenuItem("Preview", nullptr, &guiShowRenderPreview);
					
					if (ImGui::MenuItem("Start Render", "F5", nullptr, !EditorCore::IsRenderInProgress()))
					{
						EditorCore::StartFullRender(CAMERA_SETTINGS.renderWidth, CAMERA_SETTINGS.renderHeight, CAMERA_SETTINGS.fov, editorCamera, EditorObjects, LightColor);
					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Debug"))
				{
					ImGui::MenuItem("Dev Debug Console", nullptr, &guiShowDevDebugConsole);
					ImGui::MenuItem("Dear ImGui Demo", nullptr, &guiShowDearImGuiDemo);

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("View"))
				{
					ImGui::MenuItem("Gizmo", nullptr, &guiShowGizmo);
					ImGui::MenuItem("Materials", nullptr, &guiShowMaterials);

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("About"))
				{
					std::stringstream ss;
					ss << "Build date: " << __DATE__;

					ImGui::MenuItem("Reccy's Ray Tracer", nullptr, nullptr, false);
					ImGui::MenuItem("By Aaron Meaney", nullptr, nullptr, false);
					ImGui::MenuItem(ss.str().c_str(), nullptr, nullptr, false);
					
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}
		}

		// Object Properties
		{
			ImGui::SetNextWindowPos(ImVec2(imguiViewport->WorkPos.x + 10, imguiViewport->WorkPos.y + 50), ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSize(ImVec2(300, 600), ImGuiCond_FirstUseEver);
			ImGui::Begin("Object Properties");

			bool deleteObject = false;
			bool duplicateObject = false;

			bool renderThreadInProgress = EditorCore::IsRenderInProgress();

			if (renderThreadInProgress)
			{
				ImGui::Text("Render is in progress.\nCannot update properties.");
			}

			if (selectedObject == nullptr)
			{
				ImGui::Text("No object selected");
			}
			else
			{
				ImGui::BeginDisabled(renderThreadInProgress);
				ImGui::Text(("ID: " + std::to_string(selectedObject->id)).c_str()); // Inefficient string but eh it's a prototype

				if (selectedObject->objectType != EditorObject::Type::LIGHT)
				{
					deleteObject = ImGui::Button("Delete");
					duplicateObject = ImGui::Button("Duplicate");
				}
				
				ImGui::Separator();

				ImGui::InputText("Name", &selectedObject->name);

				ImGui::Separator();

				ImGui::Text("Transform");

				ImGui::Text("Position");
				if (ImGui::BeginTable("Position", 3))
				{
					const RML::Vector& pos = selectedObject->transform.position;
					double posVec[3]{ pos.x(), pos.y(), pos.z() };

					ImGui::TableNextColumn(); ImGui::InputDouble("x##Position", &posVec[0]);
					ImGui::TableNextColumn(); ImGui::InputDouble("y##Position", &posVec[1]);
					ImGui::TableNextColumn(); ImGui::InputDouble("z##Position", &posVec[2]);

					selectedObject->transform.position = RML::Vector(posVec[0], posVec[1], posVec[2]);

					ImGui::EndTable();
				}

				ImGui::BeginDisabled(selectedObject->objectType == EditorObject::Type::LIGHT);

				ImGui::Text("Rotation");
				if (ImGui::BeginTable("Rotation", 3))
				{
					const RML::Vector& rot = selectedObject->eulerRotation;
					double rotVec[3]{ rot.x(), rot.y(), rot.z() };

					bool xChanged, yChanged, zChanged = false;
					ImGui::TableNextColumn();
					xChanged = ImGui::InputDouble("x##Rotation", &rotVec[0]);
					ImGui::TableNextColumn();
					yChanged = ImGui::InputDouble("y##Rotation", &rotVec[1]);
					ImGui::TableNextColumn();
					zChanged = ImGui::InputDouble("z##Rotation", &rotVec[2]);

					if (xChanged || yChanged || zChanged)
					{
						selectedObject->transform.rotation = RML::Quaternion::euler_angles(rotVec[0], rotVec[1], rotVec[2]);
						selectedObject->eulerRotation = RML::Vector(rotVec[0], rotVec[1], rotVec[2]);
					}

					ImGui::EndTable();
				}

				ImGui::Text("Scale");
				if (ImGui::BeginTable("Scale", 3))
				{
					const RML::Vector& scale = selectedObject->transform.scaling;
					double scaleVec[3]{ scale.x(), scale.y(), scale.z() };

					ImGui::TableNextColumn(); ImGui::InputDouble("x##Scale", &scaleVec[0]);
					ImGui::TableNextColumn(); ImGui::InputDouble("y##Scale", &scaleVec[1]);
					ImGui::TableNextColumn(); ImGui::InputDouble("z##Scale", &scaleVec[2]);

					selectedObject->transform.scaling = RML::Vector(scaleVec[0], scaleVec[1], scaleVec[2]);

					ImGui::EndTable();
				}

				ImGui::EndDisabled();

				ImGui::Separator();

				std::vector<std::string> editorMaterialStrings;

				if (selectedObject->objectType == EditorObject::Type::LIGHT)
				{
					ImGui::Text("Light Properties");

					float color[3] = { LightColor.red(), LightColor.green(), LightColor.blue() };
					ImGui::ColorPicker3("Color", color);
					LightColor = Graphics::Color(color[0], color[1], color[2]);
				}
				else
				{
					if (ImGui::BeginListBox("Select Material"))
					{
						for (size_t i = 0; i < EditorMaterials.size(); i++)
						{
							const EditorMaterial* const mat = EditorMaterials[i];

							std::stringstream ss;
							ss << mat->name << "##" << i;

							std::string label = ss.str();

							if (ImGui::Selectable(label.c_str(), selectedObject->material == mat))
							{
								selectedObject->material = const_cast<EditorMaterial*>(mat);
							}
						}
					}
					ImGui::EndListBox();
				}

				ImGui::EndDisabled();
			}

			ImGui::End();

			if (deleteObject)
			{
				auto it = std::find(EditorObjects.begin(), EditorObjects.end(), selectedObject);
				delete *it;
				EditorObjects.erase(it);

				selectedObject = nullptr;
			}

			if (duplicateObject)
			{
				EditorObject* copiedObj = new EditorObject(*selectedObject);
				copiedObj->name = copiedObj->name + " (Clone)";
				EditorObjects.push_back(copiedObj);
				selectedObject = copiedObj;
			}
		}

		// Scene Objects
		{
			ImGui::Begin("Scene Objects");

			if (ImGui::BeginTable("Scene Objects", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable))
			{
				ImGui::TableSetupColumn("Name");
				ImGui::TableSetupColumn("Type");
				ImGui::TableSetupColumn("ID");
				ImGui::TableHeadersRow();

				for (const auto& objPtr : EditorObjects)
				{
					const auto& obj = *objPtr;

					bool selected = selectedObject != nullptr && obj.id == selectedObject->id;

					ImGui::TableNextRow();
					ImGui::TableNextColumn(); ImGui::Selectable(obj.name.c_str(), &selected, ImGuiSelectableFlags_SelectOnClick);
					ImGui::TableNextColumn(); ImGui::Selectable(_GetTypeName(obj.objectType).c_str(), &selected, ImGuiSelectableFlags_SelectOnClick);
					ImGui::TableNextColumn(); ImGui::Selectable(std::to_string(obj.id).c_str(), &selected, ImGuiSelectableFlags_SelectOnClick);

					if (selected)
					{
						selectedObject = const_cast<EditorObject*>(&obj);
					}
				}

				ImGui::EndTable();
			}

			ImGui::End();
		}

		// Render Dev Debug Settings
		{
			static std::string Cached = "";

			if (DebugStringStream.str().length() > 1)
			{
				Cached = DebugStringStream.str();
			}
			DebugStringStream.str("");

			if (guiShowDevDebugConsole)
			{
				ImGui::Begin("Dev Debug Console");

				ImGui::Text(Cached.c_str());
				ImGui::End();
			}
		}

		// Material Properties
		{
			if (guiShowMaterials)
			{
				bool deleteMaterial = false;
				bool duplicateMaterial = false;
				EditorMaterial* createdMaterial = nullptr;

				bool matIsSelected = SelectedMaterial != nullptr;

				if (ImGui::Begin("Materials"))
				{
					if (ImGui::BeginChild("Materials_Sidebar", ImVec2(150,0), true))
					{
						for (size_t i = 0; i < EditorMaterials.size(); i++)
						{
							const EditorMaterial* mat = EditorMaterials[i];

							std::stringstream ss;
							ss << mat->name << "##" << i;

							std::string label = ss.str();

							if (ImGui::Selectable(label.c_str(), matIsSelected && mat->id == SelectedMaterial->id))
								SelectedMaterial = const_cast<EditorMaterial*>(mat);

						}

						if (ImGui::Button("Create Material"))
						{
							createdMaterial = _CreateNewEditorMaterial();
						};
					}
					ImGui::EndChild();
					ImGui::SameLine();

					if (ImGui::BeginChild("Materials_Main") && matIsSelected)
					{
						bool renderThreadInProgress = EditorCore::IsRenderInProgress();
						bool disableEditing = renderThreadInProgress || SelectedMaterial->isProtected;

						if (SelectedMaterial->isProtected)
						{
							ImGui::Text("This material is protected.\nCannot update settings.");
						}
						else if (renderThreadInProgress)
						{
							ImGui::Text("Render is in progress.\nCannot update settings.");
						}

						ImGui::BeginDisabled(disableEditing);

						ImGui::InputText("Name", &SelectedMaterial->name);

						ImGui::BeginDisabled();
						ImGui::InputText("ID", &std::to_string(SelectedMaterial->id));
						ImGui::EndDisabled();

						ImGui::Separator();

						ImGui::Text("Surface Properties");

						ImGui::SliderFloat("Ambient", &SelectedMaterial->ambient, 0, 1);
						ImGui::SliderFloat("Diffuse", &SelectedMaterial->diffuse, 0, 1);
						ImGui::SliderFloat("Specular", &SelectedMaterial->specular, 0, 1);
						ImGui::SliderFloat("Shininess", &SelectedMaterial->shininess, 0, 1);
						ImGui::SliderFloat("Reflective", &SelectedMaterial->reflective, 0, 1);
						ImGui::SliderFloat("Transparency", &SelectedMaterial->transparency, 0, 1);
						ImGui::SliderFloat("Refractive Index", &SelectedMaterial->refractiveIndex, 0, 1);

						static const char* const shadowcastModeStrings[] = {
							"Always",
							"Never",
							"When Transparent"
						};

						int selectedShadowcastMode = static_cast<int>(SelectedMaterial->shadowcastMode);

						ImGui::ListBox("Shadowcast Mode", &selectedShadowcastMode, shadowcastModeStrings, 3);

						assert(selectedShadowcastMode >= 0);
						assert(selectedShadowcastMode <= 2);

						SelectedMaterial->shadowcastMode = static_cast<Renderer::ShadowcastMode>(selectedShadowcastMode);

						// TODO: Support multiple pattern properties
						ImGui::Separator();

						ImGui::Text("Pattern Properties");

						ImGui::BeginDisabled(true);
						static const char* const patternTypeStrings[] = {
							"Solid Color"
						};

						static int selectedPlaceholder = 0;

						ImGui::ListBox("Pattern Type", &selectedPlaceholder, patternTypeStrings, 1);
						ImGui::EndDisabled();

						SelectedMaterial->DrawPatternProperties();

						ImGui::Separator();

						deleteMaterial = ImGui::Button("Delete");

						duplicateMaterial = ImGui::Button("Duplicate");

						ImGui::EndDisabled();
					}
					ImGui::EndChild();

					ImGui::End();

					if (deleteMaterial)
					{
						auto it = std::find(EditorMaterials.begin(), EditorMaterials.end(), SelectedMaterial);

						assert(*it != &EditorCore::GetDefaultEditorMaterial());

						// Unassign Material from objects
						for (EditorObject* obj : EditorObjects)
						{
							if (obj->material == *it)
							{
								obj->material = &EditorCore::GetDefaultEditorMaterial();
							}
						}

						delete* it;
						EditorMaterials.erase(it);

						SelectedMaterial = nullptr;
					}

					if (duplicateMaterial)
					{
						EditorMaterial* copiedMat = new EditorMaterial(*SelectedMaterial);
						copiedMat->name = copiedMat->name + " (Clone)";
						copiedMat->id = EditorDB::GenerateUniqueID();
						EditorMaterials.push_back(copiedMat);
						SelectedMaterial = copiedMat;
					}

					if (createdMaterial != nullptr)
					{
						SelectedMaterial = createdMaterial;
					}
				}
			}
		}

		// Render Ray Tracer Settings
		{
			if (guiShowRenderSettings)
			{
				ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDocking;

				ImGui::Begin("Render Settings", nullptr, windowFlags);

				bool renderThreadInProgress = EditorCore::IsRenderInProgress();

				if (renderThreadInProgress)
				{
					ImGui::Text("Render is in progress.\nCannot update settings.");
				}

				ImGui::BeginDisabled(renderThreadInProgress);
				int renderDimensions[2] { CAMERA_SETTINGS.renderWidth, CAMERA_SETTINGS.renderHeight };
				ImGui::InputInt2("Render Dimensions", renderDimensions);
				CAMERA_SETTINGS.renderWidth = renderDimensions[0];
				CAMERA_SETTINGS.renderHeight = renderDimensions[1];
				ImGui::EndDisabled();
				ImGui::End();
			}
		}

		// Render Ray Tracer Preview
		{
			if (guiShowRenderPreview)
			{
				ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDocking;

				ImGui::Begin("Render Preview", nullptr, windowFlags);
				ImGui::Image((void*)raytracerRenderTextureId, ImVec2(1024, 767));
				ImGui::End();
			}
		}

		// Render Footer Bar
		{
			/*
			if (_DearImGui_BeginStatusBar())
			{
				// TODO: Find a good use for the status bar

				ImGui::MenuItem("Welcome to Reccy's Ray Tracer!", nullptr, nullptr, false);

				_DearImGui_EndStatusBar();
			}
			*/
		}

		// Render FPS Window
		{
			if (guiShowDearImGuiDemo)
				ImGui::ShowDemoWindow();
		}

		// Render Gizmo
		{
			if (guiShowGizmo)
			{
				ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus;

				_DearImGui_SetNextWindowPosRelative(ImVec2(0, 20));

				ImGui::Begin("Gizmo3D", &guiShowGizmo, windowFlags);
				ImGui::Image((void*)gizmoFramebufferId, ImVec2(128, 128), ImVec2(0,1), ImVec2(1,0));
				ImGui::End();
			}
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		//IMGUI TEST END

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backupCurrentContext);
		}

		window.SwapBuffers();
		window.PollEvents();
	}

	delete gizmoPositionMesh;
	delete gizmoRotationMesh;
	delete gizmoScaleMesh;

	delete CubeMesh;
	delete CylinderMesh;
	delete LightMesh;
	delete PlaneMesh;
	delete SphereMesh;

	delete PositionGizmo;
	delete RotationGizmo;
	delete ScaleGizmo;

	return 0;
}
