#include "editor_gui.h"

#include <string>
#include <iostream>
#include <filesystem>

#include <nfd.h>

#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_internal.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include "editor_core.h"
#include "editor_state.h"
#include "editor_io.h"
#include "ply.h"

ROGLL::Window* windowPtr = nullptr;
ImGuiViewport* imguiViewport = nullptr;

GLuint gizmoFramebufferId;
GLuint gizmoRenderTextureId;
GLuint gizmoRenderBufferId;
GLuint raytracerRenderTextureId;

ROGLL::Camera gizmoCam(32, 32, 1); // Psuedo orthograpic projection

ROGLL::Mesh* gizmoPositionMesh = nullptr;
ROGLL::Mesh* gizmoRotationMesh = nullptr;
ROGLL::Mesh* gizmoScaleMesh = nullptr;

ROGLL::MeshInstance* gizmoMeshInstance = nullptr;
ROGLL::VertexAttributes gizmoLayout;

ROGLL::Shader* gizmoShader = nullptr;
ROGLL::Material* gizmoMaterial = nullptr;

extern std::stringstream DebugStringStream;
extern EditorState State;

ROGLL::RenderBatch* gizmoBatch;

bool guiShowDearImGuiDemo = false;
bool guiShowGizmo = true;
bool guiShowMaterials = false;
bool guiIsRendering = false;
bool guiShowRenderSettings = false;
bool guiShowRenderPreview = false;
bool guiShowDevDebugConsole = false;

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

void GUI::Initialize(ROGLL::Window* window)
{
	windowPtr = window;

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

	ImGui_ImplGlfw_InitForOpenGL(window->GetHandle(), true);
	ImGui_ImplOpenGL3_Init("#version 330 core");

	imguiViewport = ImGui::GetMainViewport();

	// New Render Target Begin (For Gizmo)
	glGenFramebuffers(1, &gizmoFramebufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, gizmoFramebufferId);

	glGenTextures(1, &gizmoRenderTextureId);
	glBindTexture(GL_TEXTURE_2D, gizmoRenderTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

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
		abort;
	}

	// New Render Target End

	// New Render Target Begin (For Render)
	glGenTextures(1, &raytracerRenderTextureId);
	glBindTexture(GL_TEXTURE_2D, raytracerRenderTextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	gizmoLayout.Add<float>(ROGLL::VertexAttributes::POSITION3, 3);
	gizmoLayout.Add<float>(ROGLL::VertexAttributes::COLOR3, 3);

	gizmoShader = new ROGLL::Shader("res/shaders/Gizmo.shader");

	gizmoMaterial = new ROGLL::Material(*gizmoShader);
	gizmoMaterial->Set4("objectColor", RML::Tuple4<float>(1, 1, 1, 1));
	gizmoMaterial->Set3("handleActive", RML::Tuple3<float>(1, 1, 1));

	gizmoPositionMesh = PLY::LoadMesh("res/models/gizmo3d_position.ply", gizmoLayout);
	gizmoRotationMesh = PLY::LoadMesh("res/models/gizmo3D_rotation.ply", gizmoLayout);
	gizmoScaleMesh = PLY::LoadMesh("res/models/gizmo3d_scale.ply", gizmoLayout);

	gizmoMeshInstance = new ROGLL::MeshInstance(*gizmoPositionMesh);
	gizmoBatch = new ROGLL::RenderBatch(&gizmoLayout, gizmoMaterial);
	gizmoBatch->AddInstance(gizmoMeshInstance);

	gizmoCam.transform.translate(0, 0, -3.2);
}

void GUI::PrePass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, gizmoFramebufferId);
	glViewport(0, 0, 512, 512);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gizmoMeshInstance->transform.rotation = State.editorCamera.transform.rotation.inverse();
	gizmoBatch->Render(gizmoCam, State.lightPosition, State.GetLightColorTuple());

	if (EditorCore::IsRenderInProgress())
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, State.cameraSettings.renderWidth, State.cameraSettings.renderHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, EditorCore::GetRenderPixels());
		glBindTexture(GL_TEXTURE_2D, raytracerRenderTextureId);
	}
}

static EditorObject* _CreateCube(RML::Vector position)
{
	EditorObject* cube = new EditorObject();
	cube->id = EditorDB::GenerateUniqueID();
	cube->name = "Cube";
	cube->transform.position = position;
	cube->objectType = EditorObject::Type::CUBE;
	cube->meshInstance = EditorObject::CreateMeshInstanceForType(EditorObject::Type::CUBE);
	State.editorObjects.push_back(cube);
	return State.editorObjects[State.editorObjects.size() - 1];
}

static EditorObject* _CreatePlane(RML::Vector position)
{
	EditorObject* plane = new EditorObject();
	plane->id = EditorDB::GenerateUniqueID();
	plane->name = "Ground Plane";
	plane->transform.position = position;
	plane->objectType = EditorObject::Type::PLANE;
	plane->meshInstance = EditorObject::CreateMeshInstanceForType(EditorObject::Type::PLANE);
	State.editorObjects.push_back(plane);
	return State.editorObjects[State.editorObjects.size() - 1];
}

static EditorObject* _CreateSphere(RML::Vector position)
{
	EditorObject* sphere = new EditorObject();
	sphere->id = EditorDB::GenerateUniqueID();
	sphere->name = "Sphere";
	sphere->transform.position = position;
	sphere->objectType = EditorObject::Type::SPHERE;
	sphere->meshInstance = EditorObject::CreateMeshInstanceForType(EditorObject::Type::SPHERE);
	State.editorObjects.push_back(sphere);
	return State.editorObjects[State.editorObjects.size() - 1];
}

static EditorObject* _CreateCylinder(RML::Vector position)
{
	EditorObject* cylinder = new EditorObject();
	cylinder->id = EditorDB::GenerateUniqueID();
	cylinder->name = "Cylinder";
	cylinder->transform.position = position;
	cylinder->objectType = EditorObject::Type::CYLINDER;
	cylinder->meshInstance = EditorObject::CreateMeshInstanceForType(EditorObject::Type::CYLINDER);
	State.editorObjects.push_back(cylinder);
	return State.editorObjects[State.editorObjects.size() - 1];
}


void GUI::Draw()
{
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
						EditorIO::SaveWorld(path, State.cameraSettings, State.editorCamera, State.editorObjects, State.editorMaterials, State.lightColor);
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
						State.selectedObject = nullptr;
						State.selectedMaterial = nullptr;
						EditorIO::OpenWorld(path, State.cameraSettings, State.editorCamera, State.editorObjects, State.editorMaterials, State.lightColor);
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
					glfwSetWindowShouldClose(windowPtr->GetHandle(), 1);
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Add"))
			{
				if (ImGui::MenuItem("Cube")) {
					auto obj = _CreateCube({ 0,0,0 });
					obj->name = "New Cube";
					State.selectedObject = obj;
				}

				if (ImGui::MenuItem("Plane")) {
					auto obj = _CreatePlane({ 0, 0, 0 });
					obj->name = "New Plane";
					State.selectedObject = obj;
				}

				if (ImGui::MenuItem("Sphere")) {
					auto obj = _CreateSphere({ 0, 0, 0 });
					obj->name = "New Sphere";
					State.selectedObject = obj;
				}

				if (ImGui::MenuItem("Cylinder")) {
					auto obj = _CreateCylinder({ 0, 0, 0 });
					obj->name = "New Cylinder";
					State.selectedObject = obj;
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Render"))
			{
				ImGui::MenuItem("Settings", nullptr, &guiShowRenderSettings);
				ImGui::MenuItem("Preview", nullptr, &guiShowRenderPreview);

				if (ImGui::MenuItem("Start Render", "F5", nullptr, !EditorCore::IsRenderInProgress()))
				{
					EditorCore::StartFullRender(State.cameraSettings.renderWidth, State.cameraSettings.renderHeight, State.cameraSettings.fov, State.editorCamera, State.editorObjects, State.lightColor);
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

		if (State.selectedObject == nullptr)
		{
			ImGui::Text("No object selected");
		}
		else
		{
			ImGui::BeginDisabled(renderThreadInProgress);
			ImGui::Text(("ID: " + std::to_string(State.selectedObject->id)).c_str()); // Inefficient string but eh it's a prototype

			if (State.selectedObject->objectType != EditorObject::Type::LIGHT)
			{
				deleteObject = ImGui::Button("Delete");
				duplicateObject = ImGui::Button("Duplicate");
			}

			ImGui::Separator();

			ImGui::InputText("Name", &State.selectedObject->name);

			ImGui::Separator();

			ImGui::Text("Transform");

			ImGui::Text("Position");
			if (ImGui::BeginTable("Position", 3))
			{
				const RML::Vector& pos = State.selectedObject->transform.position;
				double posVec[3]{ pos.x(), pos.y(), pos.z() };

				ImGui::TableNextColumn(); ImGui::InputDouble("x##Position", &posVec[0]);
				ImGui::TableNextColumn(); ImGui::InputDouble("y##Position", &posVec[1]);
				ImGui::TableNextColumn(); ImGui::InputDouble("z##Position", &posVec[2]);

				State.selectedObject->transform.position = RML::Vector(posVec[0], posVec[1], posVec[2]);

				ImGui::EndTable();
			}

			ImGui::BeginDisabled(State.selectedObject->objectType == EditorObject::Type::LIGHT);

			ImGui::Text("Rotation");
			if (ImGui::BeginTable("Rotation", 3))
			{
				const RML::Vector& rot = State.selectedObject->eulerRotation;
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
					State.selectedObject->transform.rotation = RML::Quaternion::euler_angles(rotVec[0], rotVec[1], rotVec[2]);
					State.selectedObject->eulerRotation = RML::Vector(rotVec[0], rotVec[1], rotVec[2]);
				}

				ImGui::EndTable();
			}

			ImGui::Text("Scale");
			if (ImGui::BeginTable("Scale", 3))
			{
				const RML::Vector& scale = State.selectedObject->transform.scaling;
				double scaleVec[3]{ scale.x(), scale.y(), scale.z() };

				ImGui::TableNextColumn(); ImGui::InputDouble("x##Scale", &scaleVec[0]);
				ImGui::TableNextColumn(); ImGui::InputDouble("y##Scale", &scaleVec[1]);
				ImGui::TableNextColumn(); ImGui::InputDouble("z##Scale", &scaleVec[2]);

				State.selectedObject->transform.scaling = RML::Vector(scaleVec[0], scaleVec[1], scaleVec[2]);

				ImGui::EndTable();
			}

			ImGui::EndDisabled();

			ImGui::Separator();

			std::vector<std::string> editorMaterialStrings;

			if (State.selectedObject->objectType == EditorObject::Type::LIGHT)
			{
				ImGui::Text("Light Properties");

				float color[3] = { State.lightColor.red(), State.lightColor.green(), State.lightColor.blue() };
				ImGui::ColorPicker3("Color", color);
				State.lightColor = Graphics::Color(color[0], color[1], color[2]);
			}
			else
			{
				if (ImGui::BeginListBox("Select Material"))
				{
					for (size_t i = 0; i < State.editorMaterials.size(); i++)
					{
						const EditorMaterial* const mat = State.editorMaterials[i];

						std::stringstream ss;
						ss << mat->name << "##" << i;

						std::string label = ss.str();

						if (ImGui::Selectable(label.c_str(), State.selectedObject->material == mat))
						{
							State.selectedObject->material = const_cast<EditorMaterial*>(mat);
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
			auto it = std::find(State.editorObjects.begin(), State.editorObjects.end(), State.selectedObject);
			delete* it;
			State.editorObjects.erase(it);

			State.selectedObject = nullptr;
		}

		if (duplicateObject)
		{
			EditorObject* copiedObj = new EditorObject(*State.selectedObject);
			copiedObj->name = copiedObj->name + " (Clone)";
			State.editorObjects.push_back(copiedObj);
			State.selectedObject = copiedObj;
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

			for (const auto& objPtr : State.editorObjects)
			{
				const auto& obj = *objPtr;

				bool selected = State.selectedObject != nullptr && obj.id == State.selectedObject->id;

				ImGui::TableNextRow();
				ImGui::TableNextColumn(); ImGui::Selectable(obj.name.c_str(), &selected, ImGuiSelectableFlags_SelectOnClick);
				ImGui::TableNextColumn(); ImGui::Selectable(EditorObject::GetTypeName(obj.objectType).c_str(), &selected, ImGuiSelectableFlags_SelectOnClick);
				ImGui::TableNextColumn(); ImGui::Selectable(std::to_string(obj.id).c_str(), &selected, ImGuiSelectableFlags_SelectOnClick);

				if (selected)
				{
					State.selectedObject = const_cast<EditorObject*>(&obj);
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

			bool matIsSelected = State.selectedMaterial != nullptr;

			if (ImGui::Begin("Materials"))
			{
				if (ImGui::BeginChild("Materials_Sidebar", ImVec2(150, 0), true))
				{
					for (size_t i = 0; i < State.editorMaterials.size(); i++)
					{
						const EditorMaterial* mat = State.editorMaterials[i];

						std::stringstream ss;
						ss << mat->name << "##" << i;

						std::string label = ss.str();

						if (ImGui::Selectable(label.c_str(), matIsSelected && mat->id == State.selectedMaterial->id))
							State.selectedMaterial = const_cast<EditorMaterial*>(mat);
					}

					if (ImGui::Button("Create Material"))
					{
						createdMaterial = EditorMaterial::CreateInContainer(State.editorMaterials);
					};
				}
				ImGui::EndChild();
				ImGui::SameLine();

				if (ImGui::BeginChild("Materials_Main") && matIsSelected)
				{
					bool renderThreadInProgress = EditorCore::IsRenderInProgress();
					bool disableEditing = renderThreadInProgress || State.selectedMaterial->isProtected;

					if (State.selectedMaterial->isProtected)
					{
						ImGui::Text("This material is protected.\nCannot update settings.");
					}
					else if (renderThreadInProgress)
					{
						ImGui::Text("Render is in progress.\nCannot update settings.");
					}

					ImGui::BeginDisabled(disableEditing);

					ImGui::InputText("Name", &State.selectedMaterial->name);

					ImGui::BeginDisabled();
					ImGui::InputText("ID", &std::to_string(State.selectedMaterial->id));
					ImGui::EndDisabled();

					ImGui::Separator();

					ImGui::Text("Surface Properties");

					ImGui::SliderFloat("Ambient", &State.selectedMaterial->ambient, 0, 1);
					ImGui::SliderFloat("Diffuse", &State.selectedMaterial->diffuse, 0, 1);
					ImGui::SliderFloat("Specular", &State.selectedMaterial->specular, 0, 1);
					ImGui::SliderFloat("Shininess", &State.selectedMaterial->shininess, 0, 1);
					ImGui::SliderFloat("Reflective", &State.selectedMaterial->reflective, 0, 1);
					ImGui::SliderFloat("Transparency", &State.selectedMaterial->transparency, 0, 1);
					ImGui::SliderFloat("Refractive Index", &State.selectedMaterial->refractiveIndex, 0, 1);

					static const char* const shadowcastModeStrings[] = {
						"Always",
						"Never",
						"When Transparent"
					};

					int selectedShadowcastMode = static_cast<int>(State.selectedMaterial->shadowcastMode);

					ImGui::ListBox("Shadowcast Mode", &selectedShadowcastMode, shadowcastModeStrings, 3);

					assert(selectedShadowcastMode >= 0);
					assert(selectedShadowcastMode <= 2);

					State.selectedMaterial->shadowcastMode = static_cast<Renderer::ShadowcastMode>(selectedShadowcastMode);

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

					State.selectedMaterial->DrawPatternProperties();

					ImGui::Separator();

					deleteMaterial = ImGui::Button("Delete");

					duplicateMaterial = ImGui::Button("Duplicate");

					ImGui::EndDisabled();
				}
				ImGui::EndChild();

				ImGui::End();

				if (deleteMaterial)
				{
					auto it = std::find(State.editorMaterials.begin(), State.editorMaterials.end(), State.selectedMaterial);

					assert(*it != &EditorCore::GetDefaultEditorMaterial());

					// Unassign Material from objects
					for (EditorObject* obj : State.editorObjects)
					{
						if (obj->material == *it)
						{
							obj->material = &EditorCore::GetDefaultEditorMaterial();
						}
					}

					delete* it;
					State.editorMaterials.erase(it);

					State.selectedMaterial = nullptr;
				}

				if (duplicateMaterial)
				{
					EditorMaterial* copiedMat = new EditorMaterial(*State.selectedMaterial);
					copiedMat->name = copiedMat->name + " (Clone)";
					copiedMat->id = EditorDB::GenerateUniqueID();
					State.editorMaterials.push_back(copiedMat);
					State.selectedMaterial = copiedMat;
				}

				if (createdMaterial != nullptr)
				{
					State.selectedMaterial = createdMaterial;
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
			int renderDimensions[2]{ State.cameraSettings.renderWidth, State.cameraSettings.renderHeight };
			ImGui::InputInt2("Render Dimensions", renderDimensions);
			State.cameraSettings.renderWidth = renderDimensions[0];
			State.cameraSettings.renderHeight = renderDimensions[1];
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
			ImGui::Image((void*)gizmoFramebufferId, ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::End();
		}
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	//IMGUI TEST END

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backupCurrentContext);
	}
}
