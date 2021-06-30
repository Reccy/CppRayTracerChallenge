#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <Windows.h>
#include <ShlObj.h>
#include <tchar.h>
#include <chrono>

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
#include "renderer/patterns/ring.h"
#include "renderer/patterns/gradient.h"
#include "renderer/patterns/radial_gradient.h"
#include "renderer/patterns/checker.h"
#include "renderer/patterns/perturbed.h"
#include "renderer/patterns/masked.h"
#include "serializer/base_image_serializer.h"
#include "serializer/portable_pixmap_image_serializer.h"

using namespace CppRayTracerChallenge::Core;
using namespace CppRayTracerChallenge::Core::Serializer;

using namespace Renderer;
using namespace Renderer::Patterns;

using Graphics::Color;
using Graphics::Image;

void log(std::string message)
{
	std::cout << message << std::endl;
}

std::shared_ptr<Renderer::Pattern> buildFloorPattern()
{
	std::shared_ptr<Pattern> a = std::make_shared<Stripe>(Color(0.5f, 0, 0), Color(0.2f, 0, 0));
	a->transform(Transform().scale(0.05f, 1.0f, 0.05f).rotate(0, 45, 0));

	std::shared_ptr<Pattern> b = std::make_shared<Stripe>(Color(0, 0, 0.5f), Color(0, 0, 0.2f));
	b->transform(Transform().scale(0.05f, 1.0f, 0.05f).rotate(0, 45, 0));

	std::shared_ptr<Pattern> masked = std::make_shared<Masked<Checker>>(a, b);
	masked->transform(Math::Transform().rotate(0,23,0).translate(0,0.01f,0));

	std::shared_ptr<Pattern> pattern = std::make_shared<Perturbed>(masked);
	return pattern;
}

Renderer::Shape buildFloor()
{
	auto shape = std::make_shared<Plane>();
	Renderer::Shape floor = Renderer::Shape(shape);
	Transform floorTransform = Transform()
		.scale(10, 0.01, 10);
	Material bgMaterial = Material();
	bgMaterial.pattern = buildFloorPattern();
	bgMaterial.specular = 0.1f;
	floor.material(bgMaterial);
	floor.transform(floorTransform);
	return floor;
}

Renderer::Shape buildMiddleSphere()
{
	auto shape = std::make_shared<Sphere>(Sphere());
	Renderer::Shape sphere = Renderer::Shape(shape);
	Math::Transform transform = Math::Transform()
		.translate(-0.5, 1, 0.5);
	Renderer::Material material = Material();
	material.pattern = std::make_shared<SolidColor>(Color(0.1f, 1.0f, 0.5f));
	material.diffuse = 0.7f;
	material.specular = 0.3f;
	sphere.material(material);
	sphere.transform(transform);

	return sphere;
}

Renderer::Shape buildRightSphere()
{
	auto shape = std::make_shared<Sphere>(Sphere());
	Renderer::Shape sphere = Renderer::Shape(shape);
	Transform transform = Transform()
		.scale(0.5, 0.5, 0.5)
		.translate(1.5, 0.5, -0.5);
	Material material = Material();
	material.pattern = std::make_shared<Stripe>(Color(0.5f, 1.0f, 0.1f), Color(0.7f, 0.02f, 0.6f));
	material.pattern->transform(Transform().rotate(66, 283, 1).scale(2, 1.3, 0.2));
	material.diffuse = 0.7f;
	material.specular = 0.3f;
	sphere.material(material);
	sphere.transform(transform);

	return sphere;
}

Renderer::Shape buildLeftSphere()
{
	auto shape = std::make_shared<Sphere>(Sphere());
	Renderer::Shape sphere = Renderer::Shape(shape);
	Transform transform = Transform()
		.scale(0.33, 0.33, 0.33)
		.translate(-1.5, 0.33, -0.75);
	Material material = Material();
	material.pattern = std::make_shared<Stripe>(Color(1.0f, 0.8f, 0.1f), Color(0.9f, 0.2f, 0.5f));
	material.pattern->transform(Transform().rotate(33, 24, 93).scale(0.2, 1.3, 0.2));
	material.diffuse = 0.7f;
	material.specular = 0.3f;
	sphere.material(material);
	sphere.transform(transform);

	return sphere;
}

PointLight buildLight()
{
	return PointLight({ -10, 10, -10 }, Color(1, 1, 1));
}

Image doRealRender()
{
	log("Initializing...");

	log("Building Floor...");
	Renderer::Shape floor = buildFloor();

	log("Building Middle Sphere...");
	Renderer::Shape middleSphere = buildMiddleSphere();

	log("Building Right Sphere...");
	Renderer::Shape rightSphere = buildRightSphere();

	log("Building Left Sphere...");
	Renderer::Shape leftSphere = buildLeftSphere();

	log("Building Light...");
	PointLight light = buildLight();

	log("Setting up world...");
	World world = World();

	log("Adding Floor to World...");
	world.addObject(floor);

	log("Adding Middle Sphere to World...");
	world.addObject(middleSphere);

	log("Adding Right Sphere to World...");
	world.addObject(rightSphere);

	log("Adding Left Sphere to World...");
	world.addObject(leftSphere);

	log("Adding Light to World...");
	world.addLight(light);

	int width = 150;
	int height = 75;
	int fov = 75;

	log("Setting up camera: " + std::to_string(width) + ", " + std::to_string(height) + ", " + std::to_string(fov));
	Camera camera = Camera(width, height, fov);
	auto cameraTransform = Camera::viewMatrix({ 0, 1.5, -5 }, { 0,1,0 }, Vector::up());
	camera.transform(cameraTransform);

	log("Initialization Done");

	auto startTime = std::chrono::high_resolution_clock::now();

	log("Rendering scene -> Start: " + std::to_string(startTime.time_since_epoch().count()));
	auto result = camera.render(world);

	auto endTime = std::chrono::high_resolution_clock::now();
	auto elapsed = endTime - startTime;

	log("Render complete -> End: " + std::to_string(endTime.time_since_epoch().count()) + ", Elapsed: " + std::to_string(elapsed.count()));

	return result;
}

void writeImage(Image image, BaseImageSerializer& serializer)
{
	serializer.serialize(image);

	std::vector<char> ppmBuffer = serializer.buffer();

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
	Image image = doRealRender();
	PortablePixmapImageSerializer serializer;

	writeImage(image, serializer);

	return 0;
}
