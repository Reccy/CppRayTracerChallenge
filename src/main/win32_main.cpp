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
#include "math/intersections.h"
#include "math/ray.h"
#include "math/transform.h"
#include "graphics/canvas.h"
#include "graphics/color.h"
#include "renderer/world.h"
#include "renderer/material.h"
#include "renderer/point_light.h"
#include "renderer/lighting.h"
#include "renderer/sphere.h"
#include "renderer/camera.h"
#include "serializer/portable_pixmap_image_serializer.h"

using namespace CppRayTracerChallenge::Core;

Renderer::Sphere buildFloor(const Renderer::Material& mat)
{
	Renderer::Sphere floor = Renderer::Sphere();
	Math::Transform floorTransform = Math::Transform()
		.scale(10, 0.01, 10);
	floor.material(mat);
	floor.transform(floorTransform);
	return floor;
}

Renderer::Sphere buildWallLeft(const Renderer::Material& mat)
{
	Renderer::Sphere wallLeft = Renderer::Sphere();
	Math::Transform transform = Math::Transform()
		.scale(10, 0.01, 10)
		.rotate(90, 0, 0)
		.rotate(0, -45, 0)
		.translate(0, 0, 5);
	wallLeft.material(mat);
	wallLeft.transform(transform);

	return wallLeft;
}

Renderer::Sphere buildWallRight(const Renderer::Material& mat)
{
	Renderer::Sphere wallRight = Renderer::Sphere();
	Math::Transform transform = Math::Transform()
		.scale(10, 0.01, 10)
		.rotate(90, 0, 0)
		.rotate(0, 45, 0)
		.translate(0, 0, 5);
	wallRight.material(mat);
	wallRight.transform(transform);

	return wallRight;
}

Renderer::Sphere buildMiddleSphere()
{
	Renderer::Sphere sphere = Renderer::Sphere();
	Math::Transform transform = Math::Transform()
		.translate(-0.5, 1, 0.5);
	Renderer::Material material = Renderer::Material();
	material.color = Graphics::Color(0.1f, 1.0f, 0.5f);
	material.diffuse = 0.7f;
	material.specular = 0.3f;
	sphere.material(material);
	sphere.transform(transform);

	return sphere;
}

Renderer::Sphere buildRightSphere()
{
	Renderer::Sphere sphere = Renderer::Sphere();
	Math::Transform transform = Math::Transform()
		.scale(0.5, 0.5, 0.5)
		.translate(1.5, 0.5, -0.5);
	Renderer::Material material = Renderer::Material();
	material.color = Graphics::Color(0.5f, 1.0f, 0.1f);
	material.diffuse = 0.7f;
	material.specular = 0.3f;
	sphere.material(material);
	sphere.transform(transform);

	return sphere;
}

Renderer::Sphere buildLeftSphere()
{
	Renderer::Sphere sphere = Renderer::Sphere();
	Math::Transform transform = Math::Transform()
		.scale(0.33, 0.33, 0.33)
		.translate(-1.5, 0.33, -0.75);
	Renderer::Material material = Renderer::Material();
	material.color = Graphics::Color(1.0f, 0.8f, 0.1f);
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

Graphics::Canvas doRealRender()
{
	std::cout << "Rendering scene..." << std::endl;

	Renderer::Material bgMaterial = Renderer::Material();
	bgMaterial.color = Graphics::Color(1, 0.9f, 0.9f);
	bgMaterial.specular = 0;

	Renderer::Sphere floor = buildFloor(bgMaterial);
	Renderer::Sphere wallLeft = buildWallLeft(bgMaterial);
	Renderer::Sphere wallRight = buildWallRight(bgMaterial);
	Renderer::Sphere middleSphere = buildMiddleSphere();
	Renderer::Sphere rightSphere = buildRightSphere();
	Renderer::Sphere leftSphere = buildLeftSphere();
	Renderer::PointLight light = buildLight();

	Renderer::World world = Renderer::World();
	world.addObject(floor);
	world.addObject(wallLeft);
	world.addObject(wallRight);
	world.addObject(middleSphere);
	world.addObject(rightSphere);
	world.addObject(leftSphere);
	world.addLight(light);

	Renderer::Camera camera = Renderer::Camera(100, 50, 75);
	auto cameraTransform = Renderer::Camera::viewMatrix({ 0, 1.5, -5 }, { 0,1,0 }, Math::Vector::up());
	camera.transform(cameraTransform);

	Graphics::Image img = camera.render(world);

	return Graphics::Canvas(100, 50, img.toBuffer());
}

void writeCanvas(Graphics::Canvas canvas)
{
	Serializer::PortablePixmapImageSerializer ppm;
	ppm.serialize(canvas);

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
	Graphics::Canvas canvas = doRealRender();
	writeCanvas(canvas);

	return 0;
}
