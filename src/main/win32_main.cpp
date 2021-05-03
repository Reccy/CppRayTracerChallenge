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
#include "serializer/portable_pixmap_image_serializer.h"

using namespace CppRayTracerChallenge::Core;

struct Projectile {
	Math::Point position = Math::Point(0, 1, 0);
	Math::Vector velocity = Math::Vector(1, 1.8, 0).normalize() * 11.25;
};

struct Environment {
	Math::Vector gravity = Math::Vector(0, -0.1, 0);
	Math::Vector wind = Math::Vector(-0.01, 0, 0);
};

Projectile tick(const Projectile projectile, const Environment environment)
{
	Projectile result;
	result.position = projectile.position + projectile.velocity;
	result.velocity = projectile.velocity + environment.gravity + environment.wind;
	return result;
}

std::string formatTicksLog(const int currentTicks, const int maxTicks)
{
	std::stringstream ss;
	ss << "[" << currentTicks << "/" << maxTicks << "]";
	return ss.str();
}

std::string formatProjectilePositionLog(const Projectile projectile)
{
	std::stringstream ss;
	ss << "Pos: " << projectile.position;
	return ss.str();
}

std::string formatProjectileVelocityLog(const Projectile projectile)
{
	std::stringstream ss;
	ss << "Vel: " << projectile.velocity;
	return ss.str();
}

std::string formatEnvironmentGravityLog(const Environment environment)
{
	std::stringstream ss;
	ss << "Grv: " << environment.gravity;
	return ss.str();
}

std::string formatEnvironmentWindLog(const Environment environment)
{
	std::stringstream ss;
	ss << "Wnd: " << environment.wind;
	return ss.str();
}

std::string log(const int currentTicks, const int maxTicks, const Projectile projectile, const Environment environment)
{
	std::stringstream ss;
	ss << std::left << std::setw(11) << formatTicksLog(currentTicks, maxTicks);
	ss << std::left << std::setw(30) << formatProjectilePositionLog(projectile);
	ss << std::left << std::setw(30) << formatProjectileVelocityLog(projectile);
	ss << std::left << std::setw(25) << formatEnvironmentGravityLog(environment);
	ss << std::left << std::setw(25) << formatEnvironmentWindLog(environment);
	
	return ss.str();
}

float lerp(float a, float b, float f)
{
	return a + f * (b - a);
}

Graphics::Canvas createBallisticImage()
{
	Projectile projectile;
	Environment environment;

	int currentTicks = 0;
	int maxTicks = 1000;

	Graphics::Color backgroundColor = Graphics::Color::white();
	Graphics::Color foregroundColor = Graphics::Color::black();
	Graphics::Canvas canvas(900, 550, backgroundColor);

	Math::Point oldPos = projectile.position;
	Math::Point newPos = projectile.position;

	while ((projectile.position.y() > 0) && currentTicks < maxTicks)
	{
		oldPos = newPos;

		projectile = tick(projectile, environment);

		newPos = projectile.position;

		for (int i = 0; i < 100; i++)
		{
			int xPos = (int)lerp((float)oldPos.x(), (float)newPos.x(), i / 100.0f);
			int yPos = (int)lerp((float)oldPos.y(), (float)newPos.y(), i / 100.0f);

			canvas.writePixel(xPos, canvas.height() - yPos, foregroundColor);
		}

		currentTicks++;

		std::cout << log(currentTicks, maxTicks, projectile, environment) << std::endl;
	}

	return canvas;
}

Math::Point getPointForHand(int position)
{
	Math::Point point = Math::Point(0, 0, 0);

	double zRotate;
	if (position == 0)
	{
		zRotate = 0;
	}
	else
	{
		zRotate = Math::Trig::radians_to_degrees(2 * Math::Trig::PI) * (position / 12.0);
	}
	
	Math::Transform transform = Math::Transform()
		.translate(80, 0, 0) // to edge
		.rotate(0, 0, zRotate) // rotate hand
		.translate(100, 100, 0); // to origin

	return transform * point;
}

Graphics::Canvas createClock()
{
	Graphics::Color backgroundColor = Graphics::Color::black();
	Graphics::Color foregroundColor = Graphics::Color::white();
	Graphics::Canvas canvas(200, 200, backgroundColor);

	for (int i = 0; i < 12; ++i)
	{
		Math::Point draw = getPointForHand(i);

		std::cout << "Drawing at " << draw << std::endl;

		canvas.writePixel(static_cast<int>(draw.x()), static_cast<int>(draw.y()), foregroundColor);
	}
	
	return canvas;
}

Graphics::Canvas createSphereSilhouette()
{
	std::cout << "Rays are being traced..." << std::endl;

	Graphics::Color colorA = Graphics::Color(0.2f, 0.5f, 1.0f);
	Graphics::Color colorB = Graphics::Color(0.6f, 0.2f, 0.2f);
	Graphics::Color colorC = Graphics::Color(0.3f, 1.0f, 0.5f);

	Graphics::Color backgroundColor = Graphics::Color::black();
	Graphics::Canvas canvas(200, 200, backgroundColor);

	std::vector<Math::Sphere> spheres(3, Math::Sphere());
	spheres[0].transform(Math::Transform().translate(2, 0, 0).scale(1, 0.5, 0.2).rotate(0.2, 0.1, 0.1));
	spheres[1].transform(Math::Transform().translate(-1, 0, 0).scale(0.8, 1.2, 0.3).rotate(0.2, 0.3, 0.1));
	spheres[2].transform(Math::Transform().translate(0.2, -0.3, 0).scale(0.8, 1.2, 0.3).rotate(2, 0.2, 20));

	for (int px = 0; px < canvas.width(); ++px)
	{
		std::cout << "Rendering row " << std::to_string(px) << std::endl;

		for (int py = 0; py < canvas.height(); ++py)
		{
			for (int si = 0; si < spheres.size(); ++si)
			{
				Math::Ray raycast = Math::Ray({ static_cast<const double>(px - (canvas.width() / 2)) / 50, static_cast<const double>(py - (canvas.height() / 2)) / 50, 0 }, { 0, 0, 1 });
				Math::Intersections intersections = raycast.intersect_sphere(spheres[si]);

				Graphics::Color foregroundColor = Graphics::Color::white();

				if (si == 0)
				{
					foregroundColor = colorA;
				}

				if (si == 1)
				{
					foregroundColor = colorB;
				}

				if (si == 2)
				{
					foregroundColor = colorC;
				}

				if (intersections.hit().has_value())
				{
					canvas.writePixel(px, py, foregroundColor);
				}
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
