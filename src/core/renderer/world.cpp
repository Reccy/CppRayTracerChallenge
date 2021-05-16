#include "world.h"
#include "point_light.h"
#include "material.h"
#include "object.h"
#include "../math/sphere.h"
#include "../math/i_intersectable.h"
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
	mat.color = Graphics::Color(1,1,1);
	mat.diffuse = 0.7f;
	mat.specular = 0.2f;

	Math::Sphere s1 = Math::Sphere();

	Object obj1 = Object(s1, mat);

	Math::Transform transform = Math::Transform()
		.scale(0.5, 0.5, 0.5);

	Math::Sphere s2 = Math::Sphere();
	s2.transform(transform);

	Object obj2 = Object(s2, mat);

	World w = World();
	w.addLight(light);
	w.addObject(obj1);
	w.addObject(obj2);

	return w;
}

World World::addLight(PointLight light)
{
	m_lights.push_back(light);
	return *this;
}

World World::addObject(Object obj)
{
	m_objects.push_back(obj);
	return *this;
}

int World::objectCount() const
{
	return static_cast<int>(m_objects.size());
}

int World::lightCount() const
{
	return static_cast<int>(m_lights.size());
}

Math::Intersections World::intersect_ray(const Math::Ray ray) const
{
	Math::Intersections result = Math::Intersections();

	for (const Object& object : m_objects)
	{
		result += object.intersect(ray);
	}

	return result;
}
