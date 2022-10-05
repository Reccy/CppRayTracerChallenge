#include "editor_core.h"

#include "editor_io.h"
#include "editor_object.h"

#include <iostream>
#include <thread>
#include <algorithm>
#include <assert.h>

#include "math/cube.h"
#include "math/plane.h"
#include "math/sphere.h"
#include "math/cylinder.h"
#include "renderer/camera.h"
#include "graphics/color.h"
#include "serializer/portable_network_graphics_serializer.h"

namespace EditorCore
{
	using namespace CppRayTracerChallenge::Core;

	static std::vector<byte> renderData;
	static Renderer::Camera* cameraPtr = nullptr;
	static std::atomic<bool> RenderThreadInProgress(false);
	static std::thread* RenderThread;

	static ROGLL::Shader* defaultShader = nullptr;
	static ROGLL::VertexAttributes* defaultLayout = nullptr;
	static EditorMaterial* defaultMaterial = nullptr;

	Renderer::World _CreateWorld(const std::vector<EditorObject*>& editorObjects, Graphics::Color lightColor)
	{
		Renderer::World world;

		for (const auto& editorObjectPtr : editorObjects)
		{
			const auto& editorObject = *editorObjectPtr;
			Renderer::Material material = editorObject.material->CreateRendererMaterial();

			RML::Point pos(editorObject.transform.position.x(), editorObject.transform.position.y(), editorObject.transform.position.z()); // TODO: Convert point to vector in RML

			if (editorObject.objectType == EditorObjectType::LIGHT)
			{
				Renderer::PointLight light(pos, lightColor);
				world.addLight(light);
			}
			else if (editorObject.objectType == EditorObjectType::CUBE)
			{
				auto cube = std::make_shared<Math::Cube>();
				Renderer::Shape shape(cube, material);

				RML::Transform t = editorObject.transform;

				t.scaling = RML::Vector(t.scaling.x() * 0.5, t.scaling.y() * 0.5, t.scaling.z() * 0.5);
				shape.transform(t.matrix());
				world.addObject(shape);
			}
			else if (editorObject.objectType == EditorObjectType::PLANE)
			{
				auto plane = std::make_shared<Math::Plane>();
				Renderer::Shape shape(plane, material);
				shape.transform(editorObject.transform.matrix());
				world.addObject(shape);
			}
			else if (editorObject.objectType == EditorObjectType::SPHERE)
			{
				auto sphere = std::make_shared<Math::Sphere>();
				Renderer::Shape shape(sphere, material);

				RML::Transform t = editorObject.transform;

				shape.transform(t.matrix());
				world.addObject(shape);
			}
			else if (editorObject.objectType == EditorObjectType::CYLINDER)
			{
				auto cylinder = std::make_shared<Math::Cylinder>(-1, 1, true);
				Renderer::Shape shape(cylinder, material);

				RML::Transform t = editorObject.transform;

				t.scaling = t.scaling * 0.5;

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

	Graphics::Image _RenderToImage(Renderer::Camera& raytraceCamera, ROGLL::Camera& editorCamera, const std::vector<EditorObject*>& editorObjects, Graphics::Color lightColor)
	{
		std::cout << "Initializing RayTracer... ";
		Renderer::World world = _CreateWorld(editorObjects, lightColor);

		std::cout << "Configuring Camera... ";

		auto raytraceCameraViewMatrix = Renderer::Camera::viewMatrix(
			editorCamera.transform.position,
			editorCamera.transform.position + editorCamera.transform.rotation.inverse() * RML::Vector::forward(),
			RML::Vector::up()
		);

		raytraceCamera.transform(raytraceCameraViewMatrix);

		std::cout << "Done" << std::endl;

		std::cout << "Starting Render... ";

		raytraceCamera.render(world);

		std::cout << "Done" << std::endl;

		return raytraceCamera.renderedImage();
	}

	void _RenderWorkerThreadFn(Renderer::Camera& camera, ROGLL::Camera& editorCamera, const std::vector<EditorObject*>& editorObjects, Graphics::Color lightColor)
	{
		Graphics::Image image = _RenderToImage(camera, editorCamera, editorObjects, lightColor);
		Serializer::PortableNetworkGraphicsSerializer serializer;

		EditorIO::WriteImage(image, serializer);

		RenderThreadInProgress.store(false);

		std::cout << "Render Complete" << std::endl;
	}

	void StartFullRender(int renderWidth, int renderHeight, float fov, ROGLL::Camera& editorCamera, const std::vector<EditorObject*>& editorObjects, Graphics::Color lightColor)
	{
		std::cout << "Render Started" << std::endl;

		if (cameraPtr != nullptr) delete cameraPtr;

		cameraPtr = new Renderer::Camera(renderWidth, renderHeight, fov);

		RenderThreadInProgress.store(true);
		RenderThread = new std::thread(_RenderWorkerThreadFn, std::ref(*cameraPtr), std::ref(editorCamera), std::ref(editorObjects), lightColor);
	}

	byte* GetRenderPixels()
	{
		if (cameraPtr == nullptr)
		{
			return renderData.data();
		}

		auto buffer = cameraPtr->renderedImage().toBuffer();

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

	bool IsRenderInProgress()
	{
		return RenderThreadInProgress.load();
	}

	const ROGLL::Shader& GetDefaultShader()
	{
		if (defaultShader == nullptr)
		{
			defaultShader = new ROGLL::Shader("res/shaders/Default.shader");
		}

		return *defaultShader;
	}

	const ROGLL::VertexAttributes& GetDefaultLayout()
	{
		if (defaultLayout == nullptr)
		{
			defaultLayout = new ROGLL::VertexAttributes();
			defaultLayout->Add<float>(ROGLL::VertexAttributes::POSITION3, 3);
			defaultLayout->Add<float>(ROGLL::VertexAttributes::NORMAL3, 3);
		}

		return *defaultLayout;
	}

	void UpdateDefaultEditorMaterial(EditorMaterial* newEditorMaterial)
	{
		// When a scene is loaded, we need to update the ptr to the new default material

		defaultMaterial = newEditorMaterial;
	}

	const EditorMaterial& GetDefaultEditorMaterial()
	{
		if (defaultMaterial == nullptr)
		{
			EditorMaterial* mat = new EditorMaterial();
			mat->id = EditorDB::GenerateUniqueID();
			mat->name = "Default Material";
			mat->ambient = 0.1;
			mat->diffuse = 0.8;
			mat->specular = 0.1;
			mat->isProtected = true;

			defaultMaterial = mat;
		}

		return *defaultMaterial;
	}
}
