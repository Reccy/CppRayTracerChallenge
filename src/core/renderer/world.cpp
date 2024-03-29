#include "world.h"
#include "point_light.h"
#include "material.h"
#include "shape.h"
#include "lighting.h"
#include "patterns/solid_color.h"
#include "../math/sphere.h"
#include "../math/ray.h"
#include "../math/intersection.h"
#include "../math/intersections.h"
#include "../graphics/color.h"
#include "../renderer/shape.h"
#include "RML.h"

namespace CppRayTracerChallenge::Core::Renderer
{
	World::World() {};

	World World::defaultWorld()
	{
		PointLight light = PointLight({ -10, 10, -10 }, { 1, 1, 1 });

		Material mat = Material();
		mat.pattern = std::make_shared<Patterns::SolidColor>(Patterns::SolidColor(Graphics::Color(0.8f, 1.0f, 0.6f)));
		mat.diffuse = 0.7f;
		mat.specular = 0.2f;

		auto sphere1 = std::make_shared<Math::Sphere>(Math::Sphere());
		Shape s1 = Shape(sphere1, mat);

		Transform transform = Transform()
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

	World World::addObject(std::shared_ptr<Shape> obj)
	{
		m_objects.push_back(*obj);
		return *this;
	}

	World World::addGroup(Group group)
	{
		m_groups.push_back(group);
		return *this;
	}

	World World::addCSG(CSG csg)
	{
		m_csgs.push_back(csg);
		return *this;
	}

	const Shape& World::objectAt(int index) const
	{
		return m_objects.at(index);
	}

	void World::objectAt(int index, Shape shape)
	{
		m_objects.at(index) = shape;
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

		for (const Group& group : m_groups)
		{
			result += group.intersect(ray);
		}

		for (const Shape& object : m_objects)
		{
			result += object.intersect(ray);
		}

		for (const CSG& csg : m_csgs)
		{
			result += csg.intersect(ray);
		}

		return result;
	}

	Graphics::Color World::shadeHit(const ComputedValues& cv) const
	{
		return shadeHit(cv, defaultRemainingCalls);
	}

	Graphics::Color World::shadeHit(const ComputedValues& cv, int remainingCalls) const
	{
		Graphics::Color surfaceColor = Graphics::Color::black();

		for (const PointLight& light : m_lights)
		{
			bool s = isShadowed(cv.overPosition(), light);
			surfaceColor = surfaceColor + Lighting::lighting(cv.shape(), light, cv.position(), cv.eye(), cv.normal(), s);
		}

		Graphics::Color reflected = reflectedColor(cv, remainingCalls);
		Graphics::Color refracted = refractedColor(cv, remainingCalls);

		const Material& material = cv.shape().material();

		if (material.reflective > 0 && material.transparency > 0)
		{
			return surfaceColor + reflected * cv.reflectance() + refracted * (1 - cv.reflectance());
		}

		return surfaceColor + reflected + refracted;
	}

	Graphics::Color World::reflectedColor(const ComputedValues& cv) const
	{
		return reflectedColor(cv, defaultRemainingCalls);
	}

	Graphics::Color World::reflectedColor(const ComputedValues& cv, int remainingCalls) const
	{
		if (remainingCalls == 0 || cv.material().reflective == 0)
		{
			return Graphics::Color::black();
		}

		Math::Ray reflectRay = Math::Ray(cv.overPosition(), cv.reflect());
		Graphics::Color color = colorAt(reflectRay, remainingCalls - 1);

		return color * cv.material().reflective;
	}

	Graphics::Color World::refractedColor(const ComputedValues& cv) const
	{
		return refractedColor(cv, defaultRemainingCalls);
	}

	Graphics::Color World::refractedColor(const ComputedValues& cv, int remainingCalls) const
	{
		if (remainingCalls == 0)
		{
			return Graphics::Color::black();
		}

		if (cv.shape().material().transparency == 0)
		{
			return Graphics::Color::black();
		}

		float nRatio = cv.n1() / cv.n2();

		float cosI = static_cast<float>(RML::Vector::dot(cv.eye(), cv.normal()));
		float sin2t = powf(nRatio, 2.0f) * (1 - powf(cosI, 2.0f));

		if (sin2t > 1)
		{
			return Graphics::Color::black();
		}

		float cosT = sqrtf(1.0f - sin2t);

		RML::Vector direction = cv.normal() * (nRatio * cosI - cosT) - cv.eye() * nRatio;

		Math::Ray refractedRay = Math::Ray(cv.underPosition(), direction);

		return colorAt(refractedRay, remainingCalls - 1) * cv.material().transparency;
	}

	bool World::isShadowed(const RML::Point& position, const PointLight& light) const
	{
		RML::Vector positionToLight = light.position() - position;
		double distance = positionToLight.magnitude();
		RML::Vector positionToLightDirection = positionToLight.normalize();

		Math::Ray ray = Math::Ray(position, positionToLightDirection);
		Math::Intersections intersections = intersectRay(ray);

		auto hitOptional = intersections.hit();

		if (!hitOptional.has_value())
		{
			return false;
		}

		const Math::Intersection& hit = hitOptional.value();

		if (hit.t() < distance)
		{
			const Renderer::Shape& shape = static_cast<const Renderer::Shape&>(hit.shape());
			auto mode = shape.material().shadowcastMode;

			if (mode == ShadowcastMode::ALWAYS || (mode == ShadowcastMode::WHEN_TRANSPARENT && shape.material().transparency == 0))
			{
				return true;
			}
			else
			{
				RML::Point rayPosition = ray.position(hit.t() + RML::EPSILON);
				return isShadowed(rayPosition, light);
			}
		}
		else
		{
			return false;
		}
	}

	Graphics::Color World::colorAt(const Math::Ray& ray) const
	{
		return colorAt(ray, defaultRemainingCalls);
	}

	Graphics::Color World::colorAt(const Math::Ray& ray, int remainingCalls) const
	{
		Math::Intersections intersections = intersectRay(ray);

		auto hitOptional = intersections.hit();

		if (!hitOptional.has_value())
		{
			return Graphics::Color::black();
		}

		const Math::Intersection& hit = hitOptional.value();

		ComputedValues cv = ComputedValues(hit, ray);

		return shadeHit(cv, remainingCalls);
	}
}
