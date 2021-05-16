#include "sphere.h"

using namespace CppRayTracerChallenge::Core::Math;

Sphere::Sphere() : m_position(Point(0, 0, 0)) {};

Point Sphere::position() const
{
	return m_position;
}

void Sphere::transform(Transform transform)
{
	m_transform = transform;
}

const Transform Sphere::transform() const
{
	return m_transform;
}

const Vector Sphere::normal(const Point position) const
{
	Point objectPoint = m_transform.invert() * position;
	Vector objectNormal = objectPoint - m_position;
	Vector worldNormal = m_transform.invert().transpose() * objectNormal;
	return worldNormal.normalize();
}

const Intersections Sphere::intersect(const Ray& ray) const
{
	Ray rayCopy = ray.transform(transform().invert());

	Vector sphereToRay = rayCopy.origin() - position();
	double a = Vector::dot(rayCopy.direction(), rayCopy.direction());
	double b = 2 * Vector::dot(rayCopy.direction(), sphereToRay);
	double c = Vector::dot(sphereToRay, sphereToRay) - 1;

	double discriminant = b * b - 4 * a * c;

	if (discriminant < 0)
	{
		return Intersections();
	}

	double t1 = (-b - sqrt(discriminant)) / (2 * a);
	double t2 = (-b + sqrt(discriminant)) / (2 * a);

	return Intersections({ Intersection(t1, *this), Intersection(t2, *this) });
}
