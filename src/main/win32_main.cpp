#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <Windows.h>
#include <ShlObj.h>
#include <tchar.h>

#include "math/vector.h"
#include "math/point.h"
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

int main()
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

	return 0;
}
