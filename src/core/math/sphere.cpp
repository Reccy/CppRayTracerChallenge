#include "sphere.h"

using namespace CppRayTracerChallenge::Core::Math;

Sphere::Sphere() {};

const Vector Sphere::normalLocal(const Point position) const
{
	return position - Point(0, 0, 0);
}

const Intersections Sphere::intersectLocal(Ray ray) const
{
	Vector sphereToRay = ray.origin() - Point(0, 0, 0);
	double a = Vector::dot(ray.direction(), ray.direction());
	double b = 2 * Vector::dot(ray.direction(), sphereToRay);
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

const BoundingBox Sphere::bounds() const
{
	return BoundingBox(); // Not implemented
}
