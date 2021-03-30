#include <iostream>
#include <sstream>
#include <iomanip>
#include "math/tuple.h"

using namespace CppRayTracerChallenge::Core;

struct Projectile {
	Math::Tuple position = Math::Tuple::buildPoint(0, 1, 0);
	Math::Tuple velocity = Math::Tuple::buildVector(1, 1, 0).normalize();
};

struct Environment {
	Math::Tuple gravity = Math::Tuple::buildVector(0, -0.1, 0);
	Math::Tuple wind = Math::Tuple::buildVector(-0.01, 0, 0);
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

int main()
{
	Projectile projectile;
	Environment environment;

	int currentTicks = 0;
	int maxTicks = 1000;

	while ((projectile.position.getY() > 0) && currentTicks < maxTicks)
	{
		projectile = tick(projectile, environment);
		currentTicks++;

		std::cout << log(currentTicks, maxTicks, projectile, environment) << std::endl;
	}

	return 0;
}
