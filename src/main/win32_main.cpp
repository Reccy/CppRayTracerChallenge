#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <Windows.h>
#include <ShlObj.h>
#include <tchar.h>

#include "math/vector.h"
#include "math/point.h"
#include "math/transform.h"
#include "math/trig.h"
#include "math/sphere.h"
#include "math/plane.h"
#include "math/intersections.h"
#include "math/ray.h"
#include "math/transform.h"
#include "graphics/canvas.h"
#include "graphics/color.h"
#include "renderer/world.h"
#include "renderer/material.h"
#include "renderer/point_light.h"
#include "renderer/lighting.h"
#include "renderer/shape.h"
#include "renderer/camera.h"
#include "renderer/patterns/solid_color.h"
#include "renderer/patterns/stripe.h"
#include "serializer/portable_pixmap_image_serializer.h"

using namespace CppRayTracerChallenge::Core;

Renderer::Shape buildFloor(const Renderer::Material& mat)
{
	auto shape = std::make_shared<Math::Plane>(Math::Plane());
	Renderer::Shape floor = Renderer::Shape(shape);
	Math::Transform floorTransform = Math::Transform()
		.scale(10, 0.01, 10);
	floor.material(mat);
	floor.transform(floorTransform);
	return floor;
}

Renderer::Shape buildMiddleSphere()
{
	auto shape = std::make_shared<Renderer::Sphere>(Renderer::Sphere());
	Renderer::Shape sphere = Renderer::Shape(shape);
	Math::Transform transform = Math::Transform()
		.translate(-0.5, 1, 0.5);
	Renderer::Material material = Renderer::Material();
	material.pattern = std::make_shared<Renderer::Patterns::SolidColor>(Renderer::Patterns::SolidColor(Graphics::Color(0.1f, 1.0f, 0.5f)));
	material.diffuse = 0.7f;
	material.specular = 0.3f;
	sphere.material(material);
	sphere.transform(transform);

	return sphere;
}

Renderer::Shape buildRightSphere()
{
	auto shape = std::make_shared<Renderer::Sphere>(Renderer::Sphere());
	Renderer::Shape sphere = Renderer::Shape(shape);
	Math::Transform transform = Math::Transform()
		.scale(0.5, 0.5, 0.5)
		.translate(1.5, 0.5, -0.5);
	Renderer::Material material = Renderer::Material();
	material.pattern = std::make_shared<Renderer::Patterns::SolidColor>(Renderer::Patterns::SolidColor(Graphics::Color(0.5f, 1.0f, 0.1f)));
	material.diffuse = 0.7f;
	material.specular = 0.3f;
	sphere.material(material);
	sphere.transform(transform);

	return sphere;
}

Renderer::Shape buildLeftSphere()
{
	auto shape = std::make_shared<Renderer::Sphere>(Renderer::Sphere());
	Renderer::Shape sphere = Renderer::Shape(shape);
	Math::Transform transform = Math::Transform()
		.scale(0.33, 0.33, 0.33)
		.translate(-1.5, 0.33, -0.75);
	Renderer::Material material = Renderer::Material();
	material.pattern = std::make_shared<Renderer::Patterns::SolidColor>(Renderer::Patterns::SolidColor(Graphics::Color(1.0f, 0.8f, 0.1f)));
	material.diffuse = 0.7f;
	material.specular = 0.3f;
	sphere.material(material);
	sphere.transform(transform);

	return sphere;
}

Renderer::PointLight buildLight()
{
	return Renderer::PointLight({ -10, 10, -10 }, Graphics::Color(1, 1, 1));
}

Graphics::Image doRealRender()
{
	std::cout << "Rendering scene..." << std::endl;

	Renderer::Material bgMaterial = Renderer::Material();
	bgMaterial.pattern = std::make_shared<Renderer::Patterns::Stripe>(Renderer::Patterns::Stripe(Graphics::Color(1, 0.9f, 0.9f), Graphics::Color(0.6f, 0.5f, 0.4f)));
	bgMaterial.pattern->transform(Math::Transform().scale(0.25f, 1, 1).rotate(0, 45, 0));
	bgMaterial.specular = 0.1f;

	Renderer::Shape floor = buildFloor(bgMaterial);
	Renderer::Shape middleSphere = buildMiddleSphere();
	Renderer::Shape rightSphere = buildRightSphere();
	Renderer::Shape leftSphere = buildLeftSphere();
	Renderer::PointLight light = buildLight();

	Renderer::World world = Renderer::World();
	world.addObject(floor);
	world.addObject(middleSphere);
	world.addObject(rightSphere);
	world.addObject(leftSphere);
	world.addLight(light);

	Renderer::Camera camera = Renderer::Camera(100, 50, 75);
	auto cameraTransform = Renderer::Camera::viewMatrix({ 0, 1.5, -5 }, { 0,1,0 }, Math::Vector::up());
	camera.transform(cameraTransform);

	return camera.render(world);
}

void writeImage(Graphics::Image image)
{
	Serializer::PortablePixmapImageSerializer ppm;
	ppm.serialize(image);

	std::vector<char> ppmBuffer = ppm.buffer();

	std::string bufferData(ppmBuffer.begin(), ppmBuffer.end());

	TCHAR appData[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL,
		CSIDL_DESKTOPDIRECTORY | CSIDL_FLAG_CREATE,
		NULL,
		SHGFP_TYPE_CURRENT,
		appData))) {
		std::basic_ostringstream<TCHAR> filePath;
		filePath << appData << _TEXT("\\generated_image.ppm");

		std::ofstream file;
		file.open(filePath.str().c_str());

		file << bufferData << std::endl;
		file.close();

		std::cout << "File written" << std::endl;
	}
	else
	{
		std::cout << "Failed to write file" << std::endl;
	}
}

int main()
{
	Graphics::Image image = doRealRender();
	writeImage(image);

	return 0;
}
