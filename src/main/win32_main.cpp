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
#include "renderer/object.h"
#include "renderer/material.h"
#include "renderer/point_light.h"
#include "renderer/lighting.h"
#include "serializer/portable_pixmap_image_serializer.h"

using namespace CppRayTracerChallenge::Core;

Graphics::Canvas createSphereSilhouette()
{
	std::cout << "Rays are being traced..." << std::endl;

	Renderer::Material material = Renderer::Material();
	material.color = Graphics::Color(1.0f, 0.2f, 1.0f);

	Math::Sphere sphere = Math::Sphere();
	sphere.transform(Math::Transform().scale(2, 2, 2));

	Renderer::Object object = Renderer::Object(sphere, material);

	Renderer::PointLight light = Renderer::PointLight({ -10, 10, -10 }, Graphics::Color::white());

	Graphics::Color backgroundColor = Graphics::Color::black();
	Graphics::Canvas canvas(1000, 1000, backgroundColor);

	for (int px = 0; px < canvas.width(); ++px)
	{
		std::cout << "Rendering row " << std::to_string(px) << std::endl;

		for (int py = 0; py < canvas.height(); ++py)
		{
			double worldX = static_cast<const double>(px - (canvas.width() / 2)) / 50;
			double worldY = static_cast<const double>((canvas.height() - py) - (canvas.height() / 2)) / 50;
			double worldZ = -10;

			Math::Point rayPosition = Math::Point(worldX, worldY, worldZ);
			Math::Vector rayDirection = Math::Vector(0, 0, 1).normalize();

			Math::Ray raycast = Math::Ray(rayPosition, rayDirection);
			Math::Intersections intersections = object.intersect(raycast);

			if (intersections.hit().has_value())
			{
				const Math::Intersection& hit = intersections.hit().value();

				Math::Point intersectionPoint = raycast.position(hit.t());
				Math::Vector normal = hit.intersectable().normal(intersectionPoint);
				Math::Point eye = -raycast.direction();

				Graphics::Color color = Renderer::Lighting::lighting(material, light, intersectionPoint, eye, normal);

				canvas.writePixel(px, py, color);
			}
		}
	}

	return canvas;
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
	Graphics::Canvas canvas = createSphereSilhouette();
	writeCanvas(canvas);

	return 0;
}
