#include <iostream>
#include <sstream>
#include <thread>
#include <algorithm>

#define NOMINMAX
#include <windows.h>
#include <ShlObj.h>
#include <tchar.h>

#include "glad.h"
#include "glfw3.h"

#include <RPly.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_internal.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include "rogll/include.h"

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
#include <helpers/material_helper.h>

// === Learning Resources ===
// Learn OpenGL: https://learnopengl.com/
// The Cherno's OpenGL Playlist: https://youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2
// Documentation: https://docs.gl/
// Open.GL: https://open.gl/

using namespace CppRayTracerChallenge::Core;

static Renderer::Camera* RaytraceCamera;

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
static bool PerformRender = false;
static bool UnselectObject = false;
static double MousePosX = 0;
static double MousePosY = 0;
static bool MouseLeftButtonDown = false;
static bool MouseLeftButtonHeld = false;
static bool MouseLeftButtonUp = false;
static float VMove = 0;
static float HMove = 0;
static float DMove = 0;
static float RotX = 0;
static float RotY = 0;
static float RotZ = 0;
static float Fov = 90;

static float CamXRot = 0;
static float CamYRot = 0;

static int RENDER_WIDTH = 1024;
static int RENDER_HEIGHT = 768;

static int WINDOW_WIDTH = 1024;
static int WINDOW_HEIGHT = 768;

static ROGLL::Camera* MainCamera;

static struct PlyFaceStruct {
	std::vector<unsigned int>* indices = nullptr;
	unsigned int faceParseIdx = 0;
};

static enum class GizmoType
{
	POSITION,
	ROTATION,
	SCALE
};

static GizmoType CurrentGizmoType = GizmoType::POSITION;

enum class EditorObjectType
{
	CUBE,
	PLANE,
	LIGHT,
	SPHERE
};

static unsigned int _GenerateEditorObjectId()
{
	static unsigned int id = 0;
	id += 1;
	return id;
}

struct EditorMaterial
{
	bool isProtected = false;
	unsigned int id;
	std::string name;
	float ambient = 1;
	float diffuse = 0;
	float specular = 0;
	float shininess = 0;
	float reflective = 0;
	float transparency = 0;
	float refractiveIndex = 0;
	Renderer::ShadowcastMode shadowcastMode = Renderer::ShadowcastMode::WHEN_TRANSPARENT;
};

static EditorMaterial* SelectedMaterial = nullptr;

struct EditorObject
{
	unsigned int id;
	std::string name;
	RML::Transform transform;
	RML::Vector eulerRotation;
	EditorObjectType objectType;
	ROGLL::MeshInstance* meshInstance;

	EditorObject() :
		id(0),
		name(""),
		eulerRotation(),
		transform(),
		objectType(),
		meshInstance(nullptr)
	{}

	EditorObject(const EditorObject& other) :
		id(_GenerateEditorObjectId()),
		name(other.name),
		eulerRotation(other.eulerRotation),
		transform(other.transform),
		objectType(other.objectType)
	{
		meshInstance = new ROGLL::MeshInstance(*other.meshInstance);
	}

	~EditorObject()
	{
		delete meshInstance;
	}
};

static std::vector<EditorObject*> EditorObjects;
static std::vector<EditorMaterial*> EditorMaterials;

static std::stringstream DebugStringStream;

static Math::Cube SharedCube;
static Math::Plane SharedPlane;
static Math::Sphere SharedSphere;
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

static std::string _GetTypeName(EditorObjectType objectType)
{
	if (objectType == EditorObjectType::CUBE) return "Cube";
	if (objectType == EditorObjectType::LIGHT) return "Light";
	if (objectType == EditorObjectType::PLANE) return "Plane";
	if (objectType == EditorObjectType::SPHERE) return "Sphere";

	assert(false); // Should never return unknown

	return "Unknown";
}

Math::IShape* _GetMathShapeForEditorObject(const EditorObject& editorObject)
{
	if (editorObject.objectType == EditorObjectType::CUBE || editorObject.objectType == EditorObjectType::LIGHT)
	{
		RML::Transform t = editorObject.transform;
		t.scale(0.5, 0.5, 0.5);
		SharedCube.transform(t.matrix());
		return &SharedCube;
	}

	if (editorObject.objectType == EditorObjectType::PLANE)
	{
		SharedPlane.transform(editorObject.transform.matrix());
		return &SharedPlane;
	}

	if (editorObject.objectType == EditorObjectType::SPHERE)
	{
		RML::Transform t = editorObject.transform;
		SharedSphere.transform(t.matrix());
		return &SharedSphere;
	}

	assert(false); // This line should never be hit

	return nullptr;
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

struct Collider
{
	RML::Transform localTransform;
	Math::IShape* sharedShapePtr;
};

enum class Axis
{
	NONE,
	X,
	Y,
	Z
};

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

class Gizmo
{
public:
	RML::Transform transform;
public:
	Gizmo(
		const ROGLL::Mesh& mesh,
		Collider colliderA,
		Collider colliderB,
		Collider colliderC
	)
		:
		m_meshInstance(new ROGLL::MeshInstance(mesh)),
		m_colliderX(colliderA),
		m_colliderY(colliderB),
		m_colliderZ(colliderC)
	{}

	~Gizmo()
	{
		delete m_meshInstance;
	}

	Axis GetAxisForIntersectedRay(const Math::Ray& ray, RML::Vector& outHitPoint) const
	{
		Axis result = Axis::NONE;

		double closestHit = RML::INF;

		m_colliderX.sharedShapePtr->transform(transform.matrix() * m_colliderX.localTransform.matrix());
		auto xIntersect = m_colliderX.sharedShapePtr->intersect(ray);
		if (xIntersect.hit())
		{
			outHitPoint = ray.origin() + ray.direction() * xIntersect.hit().value().t();
			double hitDistance = RML::Vector(outHitPoint - ray.origin()).magnitude();
			if (hitDistance < closestHit)
			{
				closestHit = hitDistance;
				result = Axis::X;
			}
		}

		m_colliderY.sharedShapePtr->transform(transform.matrix() * m_colliderY.localTransform.matrix());
		auto yIntersect = m_colliderY.sharedShapePtr->intersect(ray);
		if (m_colliderY.sharedShapePtr->intersect(ray).hit())
		{
			outHitPoint = ray.origin() + ray.direction() * yIntersect.hit().value().t();
			double hitDistance = RML::Vector(outHitPoint - ray.origin()).magnitude();
			if (hitDistance < closestHit)
			{
				closestHit = hitDistance;
				result = Axis::Y;
			}
		}

		m_colliderZ.sharedShapePtr->transform(transform.matrix() * m_colliderZ.localTransform.matrix());
		auto zIntersect = m_colliderZ.sharedShapePtr->intersect(ray);
		if (m_colliderZ.sharedShapePtr->intersect(ray).hit())
		{
			outHitPoint = ray.origin() + ray.direction() * zIntersect.hit().value().t();
			double hitDistance = RML::Vector(outHitPoint - ray.origin()).magnitude();
			if (hitDistance < closestHit)
			{
				closestHit = hitDistance;
				result = Axis::Z;
			}
		}

		return result;
	}

	const Collider GetColliderX() const { return m_colliderX; }
	const Collider GetColliderY() const { return m_colliderY; }
	const Collider GetColliderZ() const { return m_colliderZ; }

	Gizmo(const Gizmo& other) = delete;
private:
	ROGLL::MeshInstance* m_meshInstance;
	Collider m_colliderX;
	Collider m_colliderY;
	Collider m_colliderZ;
};

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

	if (MousePosX < 0 || MousePosX > WINDOW_WIDTH || MousePosY < 0 || MousePosY > WINDOW_HEIGHT)
	{
		return result;
	}

	Math::Ray ray = camera->RayForPixel(MousePosX, MousePosY, WINDOW_WIDTH, WINDOW_HEIGHT, Fov);
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

static std::atomic<bool> RenderThreadInProgress(false);
std::thread* RenderThread;

std::vector<byte> renderData;
byte* _GetRenderPixels()
{
	if (!RaytraceCamera)
	{
		return renderData.data();
	}

	auto buffer = RaytraceCamera->renderedImage().toBuffer();

	renderData.clear();
	renderData.reserve(buffer.size() * 4);

	for (int colorIndex = 0; colorIndex < buffer.size(); ++colorIndex)
	{
		for (int componentIndex = 0; componentIndex < 4; ++componentIndex)
		{
			byte b = 0x0b;
			Graphics::Color& color = buffer.at(colorIndex);

			if (componentIndex == 0)
			{
				b = static_cast<byte>(std::clamp((int)std::ceil(color.red() * 255), 0, 255));
			}
			else if (componentIndex == 1)
			{
				b = static_cast<byte>(std::clamp((int)std::ceil(color.green() * 255), 0, 255));
			}
			else if (componentIndex == 2)
			{
				b = static_cast<byte>(std::clamp((int)std::ceil(color.blue() * 255), 0, 255));
			}
			else if (componentIndex == 3)
			{
				b = static_cast<byte>(0xFFb); // Alpha 1
			}

			renderData.push_back(b);
		}
	}

	return renderData.data();
}

void _WriteImage(Graphics::Image image, Serializer::BaseImageSerializer& serializer)
{
	std::cout << "Writing Image to Desktop... ";

	serializer.serialize(image);

	const std::vector<unsigned char> ppmBuffer = serializer.buffer();

	TCHAR appData[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL,
		CSIDL_DESKTOPDIRECTORY | CSIDL_FLAG_CREATE,
		NULL,
		SHGFP_TYPE_CURRENT,
		appData))) {
		std::basic_ostringstream<TCHAR> filePath;

		std::string imageName = std::string("\\generated_image.") + std::string(serializer.fileExtension());

		filePath << appData << _TEXT(imageName.c_str());

		std::ofstream file;
		file.open(filePath.str().c_str(), std::ios_base::binary);

		file.write((const char*)&ppmBuffer[0], ppmBuffer.size());
		file.close();

		std::cout << "Success" << std::endl;
	}
	else
	{
		std::cout << "Failed!" << std::endl;
	}
}

Renderer::Material _CreateFromEditorMaterial(const EditorMaterial* const mat)
{
	Renderer::Material result;

	result.ambient = mat->ambient;
	result.diffuse = mat->diffuse;
	result.specular = mat->specular;
	result.shininess = mat->shininess;
	result.reflective = mat->reflective;
	result.transparency = mat->transparency;
	result.refractiveIndex = mat->refractiveIndex;

	result.shadowcastMode = mat->shadowcastMode;

	return result;
}

Renderer::World _CreateWorld()
{
	Renderer::World world;

	for (const auto& editorObjectPtr : EditorObjects)
	{
		const auto& editorObject = *editorObjectPtr;

		RML::Point pos(editorObject.transform.position.x(), editorObject.transform.position.y(), editorObject.transform.position.z()); // TODO: Convert point to vector in RML

		if (editorObject.objectType == EditorObjectType::LIGHT)
		{
			Renderer::PointLight light(pos, Graphics::Color::white());
			world.addLight(light);
		}
		else if (editorObject.objectType == EditorObjectType::CUBE)
		{
			auto cube = std::make_shared<Math::Cube>();
			Renderer::Shape shape(cube);

			RML::Transform t = editorObject.transform;

			t.scaling = RML::Vector(t.scaling.x() * 0.5, t.scaling.y() * 0.5, t.scaling.z() * 0.5);
			shape.transform(t.matrix());
			world.addObject(shape);
		}
		else if (editorObject.objectType == EditorObjectType::PLANE)
		{
			auto plane = std::make_shared<Math::Plane>();
			Renderer::Shape shape(plane);
			shape.transform(editorObject.transform.matrix());
			world.addObject(shape);
		}
		else if (editorObject.objectType == EditorObjectType::SPHERE)
		{
			auto sphere = std::make_shared<Math::Sphere>();
			Renderer::Shape shape(sphere);

			RML::Transform t = editorObject.transform;

			shape.transform(t.matrix());
			world.addObject(shape);
		}
		else
		{
			assert(false);
		}
	}

	return world;
}

Graphics::Image _RenderToImage()
{
	std::cout << "Initializing RayTracer... ";
	Renderer::World world = _CreateWorld();

	std::cout << "Configuring Camera... ";

	auto raytraceCameraViewMatrix = Renderer::Camera::viewMatrix(
		MainCamera->transform.position,
		MainCamera->transform.position + MainCamera->transform.rotation.inverse() * RML::Vector::forward(),
		RML::Vector::up()
	);

	RaytraceCamera->transform(raytraceCameraViewMatrix);

	std::cout << "Done" << std::endl;

	std::cout << "Starting Render... ";

	RaytraceCamera->render(world);

	std::cout << "Done" << std::endl;

	return RaytraceCamera->renderedImage();
}

void _RenderWorkerThreadFn()
{
	Graphics::Image image = _RenderToImage();
	Serializer::PortableNetworkGraphicsSerializer serializer;

	_WriteImage(image, serializer);

	RenderThreadInProgress.store(false);

	std::cout << "Render Complete" << std::endl;
}

void _StartFullRender()
{
	std::cout << "Render Started" << std::endl;

	RaytraceCamera = new Renderer::Camera(RENDER_WIDTH, RENDER_HEIGHT, Fov);

	RenderThreadInProgress.store(true);
	RenderThread = new std::thread(_RenderWorkerThreadFn);
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

static int _PlyVertexCb(p_ply_argument argument)
{
	long attribute;
	std::vector<float>* vertexBuffer;
	ply_get_argument_user_data(argument, (void**)&vertexBuffer, &attribute);

	double data = ply_get_argument_value(argument);

	vertexBuffer->push_back(data);

	return 1;
}

static int _PlyFaceCb(p_ply_argument argument)
{
	PlyFaceStruct* plyFaceData;
	ply_get_argument_user_data(argument, (void**)&plyFaceData, NULL);
	double data = ply_get_argument_value(argument);

	if (plyFaceData->faceParseIdx % 4 == 0)
	{
		if (data == 3)
		{
			plyFaceData->faceParseIdx++;
			return 1;
		}

		std::cout << "PLY LOAD ERROR: Non Triangle face found in element list. Face size: " << data << std::endl;
		return 0;
	}

	plyFaceData->faceParseIdx++;

	plyFaceData->indices->push_back(data);

	return 1;
}

ROGLL::Mesh _LoadPlyFile(std::string filepath, const ROGLL::VertexAttributes& layout)
{
	// BEGIN GIZMO LOAD
	std::cout << "Loading ply model at [" << filepath << "]...";

	p_ply plyFile = ply_open(filepath.c_str(), NULL, 0, NULL);
	if (!plyFile || !ply_read_header(plyFile))
	{
		std::cout << "ERROR: Could not read ply file [" << filepath << "]" << std::endl;
		glfwTerminate();
		std::cin.get();
		abort();
	}

	std::vector<float> vertexPositions;
	std::vector<float> vertexNormals;
	std::vector<float> vertexColors;
	std::vector<unsigned int> indices;

	long nvertices, ntriangles;
	nvertices = ply_set_read_cb(plyFile, "vertex", "x", _PlyVertexCb, &vertexPositions, 0);
	ply_set_read_cb(plyFile, "vertex", "y", _PlyVertexCb, &vertexPositions, 1);
	ply_set_read_cb(plyFile, "vertex", "z", _PlyVertexCb, &vertexPositions, 2);
	ply_set_read_cb(plyFile, "vertex", "nx", _PlyVertexCb, &vertexNormals, 3);
	ply_set_read_cb(plyFile, "vertex", "ny", _PlyVertexCb, &vertexNormals, 4);
	ply_set_read_cb(plyFile, "vertex", "nz", _PlyVertexCb, &vertexNormals, 5);
	ply_set_read_cb(plyFile, "vertex", "red", _PlyVertexCb, &vertexColors, 6);
	ply_set_read_cb(plyFile, "vertex", "green", _PlyVertexCb, &vertexColors, 7);
	ply_set_read_cb(plyFile, "vertex", "blue", _PlyVertexCb, &vertexColors, 8);

	PlyFaceStruct pfs;
	pfs.indices = &indices;

	ntriangles = ply_set_read_cb(plyFile, "face", "vertex_indices", _PlyFaceCb, &pfs, 0);

	if (!ply_read(plyFile))
	{
		std::cout << "ERROR: Could not read ply model file" << std::endl;
		glfwTerminate();
		std::cin.get();
		abort();
	}

	ply_close(plyFile);

	std::cout << "Successfully loaded ply model file" << std::endl;

	std::vector<float> vertices;

	for (long i = 0; i < nvertices; i++)
	{
		float offset = i * 3;

		for (const auto& attrib : layout.GetAttributes())
		{
			if (attrib.tag == ROGLL::VertexAttributes::VertexAttributeTag::POSITION3)
			{
				vertices.push_back(vertexPositions.at(offset));
				vertices.push_back(vertexPositions.at(offset + 1));
				vertices.push_back(vertexPositions.at(offset + 2));
			}

			if (attrib.tag == ROGLL::VertexAttributes::VertexAttributeTag::COLOR3)
			{
				vertices.push_back(vertexColors.at(offset) / 255);
				vertices.push_back(vertexColors.at(offset + 1) / 255);
				vertices.push_back(vertexColors.at(offset + 2) / 255);
			}

			if (attrib.tag == ROGLL::VertexAttributes::VertexAttributeTag::NORMAL3)
			{
				vertices.push_back(vertexNormals.at(offset));
				vertices.push_back(vertexNormals.at(offset + 1));
				vertices.push_back(vertexNormals.at(offset + 2));
			}
		}
	}

	return ROGLL::Mesh(vertices, indices, layout);
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

	PerformRender = glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS;
	UnselectObject = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;

	bool prevMouseLeftButtonHeld = MouseLeftButtonHeld;
	MouseLeftButtonHeld = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) && !GImGui->IO.WantCaptureMouse;
	MouseLeftButtonDown = MouseLeftButtonHeld && !prevMouseLeftButtonHeld;
	MouseLeftButtonUp = !MouseLeftButtonHeld && prevMouseLeftButtonHeld;

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		CurrentGizmoType = GizmoType::POSITION;
		CurrentGizmoPtr = &PositionGizmo;
	}
	else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		CurrentGizmoType = GizmoType::ROTATION;
		CurrentGizmoPtr = &RotationGizmo;
	}
	else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		CurrentGizmoType = GizmoType::SCALE;
		CurrentGizmoPtr = &ScaleGizmo;
	}

	// IMGUI hack: We need to unfoces any fields if the user interacts with the 3D scene.
	if (MouseLeftButtonHeld) ImGui::FocusWindow(nullptr);

	glfwGetCursorPos(window, &MousePosX, &MousePosY);

	VMove = (MoveUp * 1) - (MoveDown * 1);
	HMove = (MoveRight * 1) - (MoveLeft * 1);
	DMove = (MoveForward * 1) - (MoveBackward * 1);

	RotX = (RotateXClockwise * 1) - (RotateXCounterClockwise * 1);
	RotY = (RotateYClockwise * 1) - (RotateYCounterClockwise * 1);
	RotZ = (RotateZClockwise * 1) - (RotateZCounterClockwise * 1);

	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		Fov += 0.5;

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		Fov -= 0.5;
}

void _UpdateCamera(ROGLL::Camera& cam) {
	constexpr double speedFactor = 0.25;

	cam.SetPerspective(WINDOW_WIDTH, WINDOW_HEIGHT, RML::Trig::degrees_to_radians(Fov));
	cam.transform.position += cam.transform.rotation.inverse()
		* RML::Vector(HMove * speedFactor,
			VMove * speedFactor,
			DMove * speedFactor);

	CamXRot += RotX;
	CamYRot += RotY;

	cam.transform.rotation = RML::Quaternion::euler_angles(CamXRot, CamYRot, 0);
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

static EditorMaterial* _CreateDefaultEditorMaterial()
{
	EditorMaterial* mat = new EditorMaterial();
	mat->id = _GenerateEditorObjectId();
	mat->name = "Default Material";
	mat->isProtected = true;

	assert(EditorMaterials.size() == 0); // This should be created before user defined materials

	EditorMaterials.push_back(mat);
	return EditorMaterials[0];
}

static EditorMaterial* _CreateNewEditorMaterial()
{
	EditorMaterial* mat = new EditorMaterial();
	mat->id = _GenerateEditorObjectId();
	mat->name = "New Material";
	EditorMaterials.push_back(mat);
	return EditorMaterials[EditorMaterials.size() - 1];
}

static EditorObject* _CreateLight(RML::Vector position)
{
	EditorObject* light = new EditorObject();
	light->id = _GenerateEditorObjectId();
	light->name = "Light";
	light->transform.position = position;
	light->objectType = EditorObjectType::LIGHT;
	EditorObjects.push_back(light);
	return EditorObjects[EditorObjects.size() - 1];
}

static EditorObject* _CreateCube(RML::Vector position, ROGLL::Mesh* mesh)
{
	EditorObject* cube = new EditorObject();
	cube->id = _GenerateEditorObjectId();
	cube->name = "Cube";
	cube->transform.position = position;
	cube->objectType = EditorObjectType::CUBE;
	cube->meshInstance = new ROGLL::MeshInstance(*mesh);
	EditorObjects.push_back(cube);
	return EditorObjects[EditorObjects.size() - 1];
}

static EditorObject* _CreatePlane(RML::Vector position, ROGLL::Mesh* mesh)
{
	EditorObject* plane = new EditorObject();
	plane->id = _GenerateEditorObjectId();
	plane->name = "Ground Plane";
	plane->transform.position = position;
	plane->objectType = EditorObjectType::PLANE;
	plane->meshInstance = new ROGLL::MeshInstance(*mesh);
	EditorObjects.push_back(plane);
	return EditorObjects[EditorObjects.size() - 1];
}

static EditorObject* _CreateSphere(RML::Vector position, ROGLL::Mesh* mesh)
{
	EditorObject* sphere = new EditorObject();
	sphere->id = _GenerateEditorObjectId();
	sphere->name = "Sphere";
	sphere->transform.position = position;
	sphere->objectType = EditorObjectType::SPHERE;
	sphere->meshInstance = new ROGLL::MeshInstance(*mesh);
	EditorObjects.push_back(sphere);
	return EditorObjects[EditorObjects.size() - 1];
}

int main(void)
{
	ROGLL::Window window("Reccy's Ray Tracer", WINDOW_WIDTH, WINDOW_HEIGHT);

	glfwSetWindowSizeCallback(window.GetHandle(), _WindowResized);

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

	ROGLL::VertexAttributes layout;
	layout.Add<float>(ROGLL::VertexAttributes::POSITION3, 3);
	layout.Add<float>(ROGLL::VertexAttributes::NORMAL3, 3);

	ROGLL::VertexAttributes gizmoLayout;
	gizmoLayout.Add<float>(ROGLL::VertexAttributes::POSITION3, 3);
	gizmoLayout.Add<float>(ROGLL::VertexAttributes::COLOR3, 3);

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

	auto fM = 1000.0f;

	std::vector<float> planeVertFloats
	{
		-fM, 0, -fM, 0, 1, 0,
		 fM, 0, -fM, 0, 1, 0,
		 fM, 0,  fM, 0, 1, 0,
		-fM, 0,  fM, 0, 1, 0
	};

	ROGLL::Mesh planeMesh(
		planeVertFloats,
		{
			0, 1, 2,
			2, 3, 0,
		},
		layout
		);

	ROGLL::Mesh sphereMesh = _LoadPlyFile("res/models/sphere.ply", layout);

	ROGLL::Mesh gizmoPositionMesh = _LoadPlyFile("res/models/gizmo3d_position.ply", gizmoLayout);
	ROGLL::Mesh gizmoRotationMesh = _LoadPlyFile("res/models/gizmo3D_rotation.ply", gizmoLayout);
	ROGLL::Mesh gizmoScaleMesh = _LoadPlyFile("res/models/gizmo3d_scale.ply", gizmoLayout);

	ROGLL::MeshInstance gizmoMeshInstance(gizmoPositionMesh);
	ROGLL::MeshInstance positionHandleMeshInstance(gizmoPositionMesh);
	ROGLL::MeshInstance rotationHandleMeshInstance(gizmoRotationMesh);
	ROGLL::MeshInstance scaleHandleMeshInstance(gizmoScaleMesh);

	double offsetFromOrigin = 0.73;
	double width = 0.1;
	double length = 0.73;

	PositionGizmo = new Gizmo(gizmoPositionMesh,
		{ RML::Transform().translate(offsetFromOrigin, 0, 0).scale(length, width, width), &SharedCube},
		{ RML::Transform().translate(0, offsetFromOrigin, 0).scale(width, length, width), &SharedCube},
		{ RML::Transform().translate(0, 0, offsetFromOrigin).scale(width, width, length), &SharedCube});
	
	RotationGizmo = new Gizmo(gizmoRotationMesh,
		{ RML::Transform().rotate(90, 90, 0), &SharedTorus},
		{ RML::Transform(), &SharedTorus},
		{ RML::Transform().rotate(90, 0, 0), &SharedTorus});

	ScaleGizmo = new Gizmo(gizmoScaleMesh,
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

	ROGLL::VertexAttributes uiTextureLayout;
	uiTextureLayout.Add<float>(ROGLL::VertexAttributes::POSITION3, 3);
	uiTextureLayout.Add<float>(ROGLL::VertexAttributes::UV2, 2);

	ROGLL::Mesh uiTextureMesh(
		uiTextureFloats,
		{
			0, 1, 2,
			0, 2, 3
		},
		uiTextureLayout);
	ROGLL::MeshInstance gizmoUiInstance(uiTextureMesh);

	ROGLL::Shader defaultShader("res/shaders/Default.shader");
	ROGLL::Shader gizmoShader("res/shaders/Gizmo.shader");

	ROGLL::Material gizmoMaterial(gizmoShader);
	gizmoMaterial.Set4("objectColor", White);
	gizmoMaterial.Set3("handleActive", RML::Tuple3<float>( 1, 1, 1 ));

	ROGLL::Material handleMaterial(gizmoShader);
	handleMaterial.Set4("objectColor", White);
	handleMaterial.Set3("handleActive", RML::Tuple3<float>( 0, 0, 0 ));

	ROGLL::Material cubeMaterial(defaultShader);
	cubeMaterial.Set4("objectColor", Blue);

	ROGLL::Material planeMaterial(defaultShader);
	planeMaterial.Set4("objectColor", Green);

	ROGLL::Material sphereMaterial(defaultShader);
	sphereMaterial.Set4("objectColor", White);

	ROGLL::Material debugMeshMaterial(defaultShader);
	debugMeshMaterial.Set4("objectColor", Red + Green);

	ROGLL::RenderBatch cubeBatch(&layout, &cubeMaterial);

	ROGLL::RenderBatch gizmoBatch(&gizmoLayout, &gizmoMaterial);
	gizmoBatch.AddInstance(&gizmoMeshInstance);

	ROGLL::MeshInstance* currentHandleMeshInstance = &positionHandleMeshInstance;

	ROGLL::RenderBatch handleBatch(&gizmoLayout, &handleMaterial);

	ROGLL::RenderBatch planeBatch(&layout, &planeMaterial);

	ROGLL::RenderBatch sphereBatch(&layout, &sphereMaterial);

	ROGLL::RenderBatch debugBatch(&layout, &debugMeshMaterial);

	ROGLL::MeshInstance debugMeshInstance2(cubeMesh);
	debugMeshInstance2.transform.scale(0.4, 0.4, 0.4);
	// debugBatch.AddInstance(&debugMeshInstance2);

	ROGLL::MeshInstance debugMeshInstance(cubeMesh);
	debugMeshInstance.transform.scale(0.2, 0.2, 0.2);
	// debugBatch.AddInstance(&debugMeshInstance);

	ROGLL::Camera cam(WINDOW_WIDTH, WINDOW_HEIGHT, 60);
	cam.transform.translate(0, 0, -10); // Initial cam position

	MainCamera = &cam;

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

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window.GetHandle(), true);
	ImGui_ImplOpenGL3_Init("#version 330 core");
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

	auto light = _CreateLight({ 0, 10, 0 });
	auto cubeA = _CreateCube({0, 0.5, 0}, &cubeMesh);
	cubeA->name = "Cube A";

	auto cubeB = _CreateCube({ 1, 1.5, 0 }, &cubeMesh);
	cubeB->name = "Cube B";

	auto ground = _CreatePlane({ 0,0,0 }, &planeMesh);
	ground->name = "Ground Plane";

	// New Render Target End

	bool guiShowDearImGuiDemo = false;
	bool guiShowGizmo = true;
	bool guiShowMaterials = false;
	bool guiIsRendering = false;
	bool guiShowRenderSettings = false;
	bool guiShowRenderPreview = false;
	bool guiShowDevDebugConsole = false;
	bool guiPaulMode = false;

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

	_CreateDefaultEditorMaterial();

	while (!window.ShouldClose())
	{
		_ProcessInput(window);

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

		if (PerformRender && !RenderThreadInProgress.load())
		{
			_StartFullRender();
		}

		ObjectPickerHit selectedObjectHit = _SelectObjectUnderCursor(MainCamera, selectedObject != nullptr);

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

		if (MouseLeftButtonDown)
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

			const Math::Ray ray = MainCamera->RayForPixel(MousePosX, MousePosY, WINDOW_WIDTH, WINDOW_HEIGHT, Fov);
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
					const RML::Vector globalPositiveAxis = [&]() {
						switch (selectedObjectHitAxis)
						{
						case Axis::X: return RML::Vector::right();
						case Axis::Y: return RML::Vector::up();
						case Axis::Z: return RML::Vector::forward();
						}
					}();

					const RML::Vector localAxis = [&]() {
						switch (selectedObjectHitAxis)
						{
						case Axis::X: return selectedObject->transform.right();
						case Axis::Y: return selectedObject->transform.up();
						case Axis::Z: return selectedObject->transform.forward();
						}
					}();

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

		if (MouseLeftButtonUp)
		{
			if (selectedObjectHitAxis != Axis::NONE)
			{
				selectedObjectHitAxis = Axis::NONE;
			}
		}

		if (UnselectObject)
		{
			selectedObject = nullptr;
		}

		// Only update the camera if the user has not selected a handle axis
		// This is to prevent the camera moving during drag and breaking the cursor delta calculation
		if (selectedObjectHitAxis == Axis::NONE)
		{
			_UpdateCamera(cam);
		}

		// UI - Pre Pass
		glBindFramebuffer(GL_FRAMEBUFFER, gizmoFramebufferId);
		glViewport(0, 0, 512, 512);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		gizmoMeshInstance.transform.rotation = cam.transform.rotation.inverse();
		gizmoBatch.Render(gizmoCam, lightPosition);

		if (RenderThreadInProgress.load())
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, RENDER_WIDTH, RENDER_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, _GetRenderPixels());
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

			if (obj.objectType == EditorObjectType::LIGHT)
			{
				lightPosition = RML::Tuple3<float>(obj.transform.position.x(), obj.transform.position.y(), obj.transform.position.z());
				continue;
			}

			obj.meshInstance->transform = obj.transform;

			if (obj.objectType == EditorObjectType::CUBE)
			{
				cubeBatch.AddInstance(obj.meshInstance);
			}
			else if (obj.objectType == EditorObjectType::PLANE)
			{
				planeBatch.AddInstance(obj.meshInstance);
			}
			else if (obj.objectType == EditorObjectType::SPHERE)
			{
				sphereBatch.AddInstance(obj.meshInstance);
			}
		}

		cubeBatch.Render(cam, lightPosition);
		planeBatch.Render(cam, lightPosition);
		sphereBatch.Render(cam, lightPosition);
		debugBatch.Render(cam, lightPosition);

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
			
			handleBatch.Render(cam, lightPosition);
		}

		cubeBatch.Clear();
		planeBatch.Clear();
		sphereBatch.Clear();
		handleBatch.Clear();

		//IMGUI TEST BEGIN

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Render Main Menu Bar
		{
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Exit", "Alt + F4")) {
						glfwSetWindowShouldClose(window.GetHandle(), 1);
					}
					
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Add"))
				{
					if (ImGui::MenuItem("Cube")) {
						auto obj = _CreateCube({ 0,0,0 }, &cubeMesh);
						obj->name = "New Cube";
						selectedObject = obj;
					}

					if (ImGui::MenuItem("Plane")) {
						auto obj = _CreatePlane({ 0, 0, 0 }, &planeMesh);
						obj->name = "New Plane";
						selectedObject = obj;
					}

					if (ImGui::MenuItem("Sphere")) {
						auto obj = _CreateSphere({ 0, 0, 0 }, &sphereMesh);
						obj->name = "New Sphere";
						selectedObject = obj;
					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Render"))
				{
					ImGui::MenuItem("Settings", nullptr, &guiShowRenderSettings);
					ImGui::MenuItem("Preview", nullptr, &guiShowRenderPreview);
					
					if (ImGui::MenuItem("Start Render", "F5", nullptr, !RenderThreadInProgress.load()))
					{
						_StartFullRender();
					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Debug"))
				{
					ImGui::MenuItem("Dev Debug Console", nullptr, &guiShowDevDebugConsole);
					ImGui::MenuItem("Dear ImGui Demo", nullptr, &guiShowDearImGuiDemo);
					ImGui::MenuItem("Enable Paul Mode", nullptr, &guiPaulMode);

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
					ImGui::MenuItem("Reccy's Ray Tracer", nullptr, nullptr, false);
					ImGui::MenuItem("By Aaron Meaney", nullptr, nullptr, false);
					ImGui::MenuItem("Build date: 18/08/2022", nullptr, nullptr, false); // TODO: Replace with compile time variable
					
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}
		}

		// Object Properties
		{
			ImGui::Begin("Object Properties");

			bool deleteObject = false;
			bool duplicateObject = false;

			bool renderThreadInProgress = RenderThreadInProgress.load();

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

				if (selectedObject->objectType != EditorObjectType::LIGHT)
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
						for (const EditorMaterial* mat : EditorMaterials)
						{
							if (ImGui::Selectable(mat->name.c_str(), matIsSelected && mat->id == SelectedMaterial->id))
								SelectedMaterial = const_cast<EditorMaterial*>(mat);
						}

						if (ImGui::Button("Create Material"))
						{
							createdMaterial = _CreateNewEditorMaterial();
						};
					}
					ImGui::EndChild();
					ImGui::SameLine();

					if (matIsSelected && ImGui::BeginChild("Materials_Main"))
					{
						bool renderThreadInProgress = RenderThreadInProgress.load();
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

						ImGui::Separator();

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

						deleteMaterial = ImGui::Button("Delete");
						duplicateMaterial = ImGui::Button("Duplicate");

						ImGui::EndDisabled();
					}
					ImGui::EndChild();

					ImGui::End();

					if (deleteMaterial)
					{
						auto it = std::find(EditorMaterials.begin(), EditorMaterials.end(), SelectedMaterial);
						delete* it;
						EditorMaterials.erase(it);

						SelectedMaterial = nullptr;
					}

					if (duplicateMaterial)
					{
						EditorMaterial* copiedMat = new EditorMaterial(*SelectedMaterial);
						copiedMat->name = copiedMat->name + " (Clone)";
						copiedMat->id = _GenerateEditorObjectId();
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

				bool renderThreadInProgress = RenderThreadInProgress.load();

				if (renderThreadInProgress)
				{
					ImGui::Text("Render is in progress.\nCannot update settings.");
				}

				ImGui::BeginDisabled(renderThreadInProgress);
				int renderDimensions[2] { RENDER_WIDTH, RENDER_HEIGHT };
				ImGui::InputInt2("Render Dimensions", renderDimensions);
				RENDER_WIDTH = renderDimensions[0];
				RENDER_HEIGHT = renderDimensions[1];
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
			if (_DearImGui_BeginStatusBar())
			{
				std::stringstream camPosSS;

				if (guiPaulMode)
				{
					camPosSS << "Camera Position: X: " << cam.transform.position.x() << " Y: " << cam.transform.position.y() << " Z: " << cam.transform.position.z();
				}
				else
				{
					camPosSS << "Camera Position: " << cam.transform.position.x() << "x " << cam.transform.position.y() << "y " << cam.transform.position.z() << "z";
				}

				camPosSS << " | ";
				camPosSS << "FOV: " << Fov << " degrees";
				camPosSS << " | ";
				camPosSS << "Mouse Pos: " << MousePosX << ", " << MousePosY;
				camPosSS << " | ";

				ImGui::MenuItem(camPosSS.str().c_str(), nullptr, nullptr, false);

				_DearImGui_EndStatusBar();
			}
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

	delete PositionGizmo;
	delete RotationGizmo;
	delete ScaleGizmo;

	return 0;
}
