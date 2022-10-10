#include "editor_gizmo.h"

Gizmo::Gizmo(
	const ROGLL::Mesh& mesh,
	Collider colliderA,
	Collider colliderB,
	Collider colliderC
)
	:
	m_meshInstance(new ROGLL::MeshInstance(mesh)),
	m_colliderX(colliderA),
	m_colliderY(colliderB),
	m_colliderZ(colliderC)
{}

Gizmo::~Gizmo()
{
	delete m_meshInstance;
}

Axis Gizmo::GetAxisForIntersectedRay(const CppRayTracerChallenge::Core::Math::Ray& ray, RML::Vector& outHitPoint) const
{
	Axis result = Axis::NONE;

	double closestHit = RML::INF;

	m_colliderX.sharedShapePtr->transform(transform.matrix() * m_colliderX.localTransform.matrix());
	auto xIntersect = m_colliderX.sharedShapePtr->intersect(ray);
	if (xIntersect.hit())
	{
		outHitPoint = ray.origin() + ray.direction() * xIntersect.hit().value().t();
		double hitDistance = RML::Vector(outHitPoint - ray.origin()).magnitude();
		if (hitDistance < closestHit)
		{
			closestHit = hitDistance;
			result = Axis::X;
		}
	}

	m_colliderY.sharedShapePtr->transform(transform.matrix() * m_colliderY.localTransform.matrix());
	auto yIntersect = m_colliderY.sharedShapePtr->intersect(ray);
	if (m_colliderY.sharedShapePtr->intersect(ray).hit())
	{
		outHitPoint = ray.origin() + ray.direction() * yIntersect.hit().value().t();
		double hitDistance = RML::Vector(outHitPoint - ray.origin()).magnitude();
		if (hitDistance < closestHit)
		{
			closestHit = hitDistance;
			result = Axis::Y;
		}
	}

	m_colliderZ.sharedShapePtr->transform(transform.matrix() * m_colliderZ.localTransform.matrix());
	auto zIntersect = m_colliderZ.sharedShapePtr->intersect(ray);
	if (m_colliderZ.sharedShapePtr->intersect(ray).hit())
	{
		outHitPoint = ray.origin() + ray.direction() * zIntersect.hit().value().t();
		double hitDistance = RML::Vector(outHitPoint - ray.origin()).magnitude();
		if (hitDistance < closestHit)
		{
			closestHit = hitDistance;
			result = Axis::Z;
		}
	}

	return result;
}

Collider Gizmo::GetColliderX() const { return m_colliderX; }
Collider Gizmo::GetColliderY() const { return m_colliderY; }
Collider Gizmo::GetColliderZ() const { return m_colliderZ; }
