#include "world.h"
#include "point_light.h"
#include "material.h"
#include "shape.h"
#include "lighting.h"
#include "../math/sphere.h"
#include "../math/transform.h"
#include "../math/ray.h"
#include "../math/intersection.h"
#include "../math/intersections.h"
#include "../graphics/color.h"

using namespace CppRayTracerChallenge::Core::Renderer;

World::World() {};

World World::defaultWorld()
{
	PointLight light = PointLight({ -10, 10, -10 }, { 1, 1, 1 });

	Material mat = Material();
	mat.color = Graphics::Color(0.8f, 1.0f, 0.6f);
	mat.diffuse = 0.7f;
	mat.specular = 0.2f;

	auto sphere1 = std::make_shared<Math::Sphere>(Math::Sphere());
	Shape s1 = Shape(sphere1, mat);

	Math::Transform transform = Math::Transform()
		.scale(0.5, 0.5, 0.5);

	auto sphere2 = std::make_shared<Math::Sphere>(Math::Sphere());
	Shape s2 = Shape(sphere2);
	s2.transform(transform);

	World w = World();
	w.addLight(light);
	w.addObject(s1);
	w.addObject(s2);

	return w;
}

World World::addLight(PointLight light)
{
	m_lights.push_back(light);
	return *this;
}

const PointLight& World::lightAt(int index) const
{
	return m_lights.at(index);
}

World World::clearLights()
{
	m_lights.clear();
	return *this;
}

World World::addObject(Shape obj)
{
	m_objects.push_back(obj);
	return *this;
}

const Shape& World::objectAt(int index) const
{
	return m_objects.at(index);
}

int World::objectCount() const
{
	return static_cast<int>(m_objects.size());
}

int World::lightCount() const
{
	return static_cast<int>(m_lights.size());
}

Math::Intersections World::intersectRay(const Math::Ray ray) const
{
	Math::Intersections result = Math::Intersections();

	for (const Shape& object : m_objects)
	{
		result += Math::Intersections::intersect(ray, object);
	}

	return result;
}

Graphics::Color World::shadeHit(const ComputedValues& cv) const
{
	Graphics::Color color = Graphics::Color::black();

	for (const PointLight& light : m_lights)
	{
		bool s = isShadowed(cv.overPosition(), light);
		color = color + Lighting::lighting(cv.material(), light, cv.position(), cv.eye(), cv.normal(), s);
	}

	return color;
}

bool World::isShadowed(const Math::Point& position, const PointLight& light) const
{
	Math::Vector positionToLight = light.position() - position;
	double distance = positionToLight.magnitude();
	Math::Vector positionToLightDirection = positionToLight.normalize();

	Math::Ray ray = Math::Ray(position, positionToLightDirection);
	Math::Intersections intersections = intersectRay(ray);

	if (!intersections.hit().has_value())
	{
		return false;
	}

	if (intersections.hit().value().t() < distance)
	{
		return true;
	}
	else
	{
		return false;
	}
}

Graphics::Color World::colorAt(const Math::Ray& ray) const
{
	Math::Intersections intersections = intersectRay(ray);

	if (!intersections.hit().has_value())
	{
		return Graphics::Color::black();
	}

	const Math::Intersection& hit = intersections.hit().value();

	ComputedValues cv = ComputedValues(hit, ray);

	return shadeHit(cv);
}
