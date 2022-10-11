#include <iostream>
#include <sstream>
#include <thread>
#include <algorithm>
#include <random>
#include <map>
#include <filesystem>
#include <assert.h>

#define NOMINMAX
#include <windows.h>
#include <ShlObj.h>
#include <tchar.h>

#include "glad.h"
#include "glfw3.h"

#include <tinyxml2.h>

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
#include "src/editor_gui.h"
#include "src/editor_state.h"
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

EditorState State;

std::stringstream DebugStringStream;

static RML::Tuple4<float> Red{ 1.0f, 0.0f, 0.0f, 1.0f };
static RML::Tuple4<float> Green{ 0.0f, 1.0f, 0.0f, 1.0f };
static RML::Tuple4<float> Blue{ 0.0f, 0.0f, 1.0f, 1.0f };
static RML::Tuple4<float> White{ 1.0f, 1.0f, 1.0f, 1.0f };
static RML::Tuple4<float> Black{ 0.0f, 0.0f, 0.0f, 0.0f };

static RML::Tuple4<float>* ClearColor = &Black;

static EditorActions Actions;

static int WINDOW_WIDTH = 1024;
static int WINDOW_HEIGHT = 768;

static ROGLL::Camera* MainCamera;

static GizmoType CurrentGizmoType = GizmoType::POSITION;

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

	Math::Ray ray = camera->RayForPixel(Actions.MousePosX, Actions.MousePosY, WINDOW_WIDTH, WINDOW_HEIGHT, State.cameraSettings.fov);
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

	for (const auto& editorObject : State.editorObjects)
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

void _UpdateCamera(ROGLL::Camera& cam) {
	constexpr double speedFactor = 0.25;

	cam.SetPerspective(WINDOW_WIDTH, WINDOW_HEIGHT, RML::Trig::degrees_to_radians(State.cameraSettings.fov));
	cam.transform.position += cam.transform.rotation.inverse()
		* RML::Vector(Actions.HMove * speedFactor,
			Actions.VMove * speedFactor,
			Actions.DMove * speedFactor);

	State.cameraSettings.xRot += Actions.RotX;
	State.cameraSettings.yRot += Actions.RotY;

	constexpr float camXRotLimit = 89;

	State.cameraSettings.xRot = std::clamp(State.cameraSettings.xRot, -camXRotLimit, camXRotLimit);

	cam.transform.rotation = RML::Quaternion::euler_angles(State.cameraSettings.xRot, State.cameraSettings.yRot, 0);
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

static EditorObject* _CreateLight(RML::Vector position)
{
	EditorObject* light = new EditorObject();
	light->id = EditorDB::GenerateUniqueID();
	light->name = "Light";
	light->transform.position = position;
	light->objectType = EditorObject::Type::LIGHT;
	light->meshInstance = EditorObject::CreateMeshInstanceForType(EditorObject::Type::LIGHT);
	State.editorObjects.push_back(light);
	return State.editorObjects[State.editorObjects.size() - 1];
}

// These are used globally in EditorObject.cpp
ROGLL::Mesh* CubeMesh;
ROGLL::Mesh* CylinderMesh;
ROGLL::Mesh* LightMesh;
ROGLL::Mesh* PlaneMesh;
ROGLL::Mesh* SphereMesh;

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

	ROGLL::Shader gizmoShader("res/shaders/Gizmo.shader"); // TODO: Allow Shader class to cache loaded shaders.
	ROGLL::Shader lightShader("res/shaders/Light.shader");

	ROGLL::Material handleMaterial(gizmoShader);
	handleMaterial.Set4("objectColor", White);
	handleMaterial.Set3("handleActive", RML::Tuple3<float>( 0, 0, 0 ));

	ROGLL::Material lightMaterial(lightShader);
	lightMaterial.Set4("objectColor", RML::Tuple4<float>(0.1f, 0.2f, 0.1f, 1.0f));

	ROGLL::Material debugMeshMaterial(EditorCore::GetDefaultShader());
	debugMeshMaterial.Set4("objectColor", Red + Green);

	ROGLL::MeshInstance* currentHandleMeshInstance = &positionHandleMeshInstance;

	ROGLL::RenderBatch handleBatch(&gizmoLayout, &handleMaterial);

	ROGLL::RenderBatch lightBatch(&lightLayout, &lightMaterial);

	State.editorCamera.SetPerspective(WINDOW_WIDTH, WINDOW_HEIGHT, 60);

	State.editorCamera.transform.translate(0, 0, -10); // Initial cam position

	MainCamera = &State.editorCamera;

	State.lightPosition = {
		1,
		9,
		0
	};

	GUI::Initialize(&window);

	auto light = _CreateLight({ 0, 0, 0 });

	// New Render Target End

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
			State.cameraSettings.fov += 0.5;
		}

		if (Actions.DecreaseFov)
		{
			State.cameraSettings.fov -= 0.5;
		}

		State.cameraSettings.fov = std::clamp(State.cameraSettings.fov, 5.0f, 170.0f);

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
			EditorCore::StartFullRender(State.cameraSettings.renderWidth, State.cameraSettings.renderHeight, State.cameraSettings.fov, State.editorCamera, State.editorObjects, State.lightColor);
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

			const Math::Ray ray = MainCamera->RayForPixel(Actions.MousePosX, Actions.MousePosY, WINDOW_WIDTH, WINDOW_HEIGHT, State.cameraSettings.fov);
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
			_UpdateCamera(State.editorCamera);
		}

		GUI::PrePass();

		// Geometry Pass
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glClearColor(ClearColor->x(), ClearColor->y(), ClearColor->z(), ClearColor->w());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// Update the mesh instance transforms and set up render batches (since they are different to the editor object transform)
		for (const auto& objPtr : State.editorObjects)
		{
			const auto& obj = *objPtr;

			if (obj.objectType == EditorObject::Type::LIGHT)
			{
				State.lightPosition = RML::Tuple3<float>(obj.transform.position.x(), obj.transform.position.y(), obj.transform.position.z());
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

		for (auto& material : State.editorMaterials)
		{
			// TODO: Support multiple patterns
			Graphics::Color c = static_cast<EditorPatternSolidColor*>(material->editorPattern)->color;
			material->previewMaterial->Set4("objectColor", RML::Tuple4<float>(c.red(), c.green(), c.blue(), 1));
			material->renderBatch->Render(State.editorCamera, State.lightPosition, State.GetLightColorTuple());
		}

		lightBatch.Render(State.editorCamera, State.lightPosition, State.GetLightColorTuple());

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
			
			handleBatch.Render(State.editorCamera, State.lightPosition, State.GetLightColorTuple());
		}

		for (auto& material : State.editorMaterials)
		{
			material->renderBatch->Clear();
		}

		lightBatch.Clear();
		handleBatch.Clear();

		GUI::Draw();

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
