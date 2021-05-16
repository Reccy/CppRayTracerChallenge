#include "world.h"
#include "point_light.h"
#include "sphere.h"
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

	Sphere s1 = Sphere();
	s1.material(mat);

	Math::Transform transform = Math::Transform()
		.scale(0.5, 0.5, 0.5);

	Sphere s2 = Sphere();
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

World World::addObject(Sphere obj)
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

	for (const Sphere& sphere : m_objects)
	{
		result += sphere.intersect(ray);
	}

	return result;
}
