#include "world.h"
#include "point_light.h"
#include "material.h"
#include "sphere.h"
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

	Sphere s1 = Sphere(mat);

	Math::Transform transform = Math::Transform()
		.scale(0.5, 0.5, 0.5);

	Renderer::Sphere s2 = Renderer::Sphere();
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

World World::addObject(Sphere obj)
{
	m_objects.push_back(obj);
	return *this;
}

const Sphere& World::objectAt(int index) const
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

	for (const Sphere& object : m_objects)
	{
		result += object.intersect(ray);
	}

	return result;
}

Graphics::Color World::shadeHit(const ComputedValues& cv) const
{
	Graphics::Color color = Graphics::Color::black();

	for (const PointLight& light : m_lights)
	{
		color = color + Lighting::lighting(cv.shape().material(), light, cv.position(), cv.eye(), cv.normal());
	}

	return color;
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
