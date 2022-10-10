#pragma once

#include "rogll/include.h"
#include "math/i_shape.h"
#include "editor_axis.h"

enum class GizmoType
{
	POSITION,
	ROTATION,
	SCALE
};

struct Collider
{
	RML::Transform localTransform;
	CppRayTracerChallenge::Core::Math::IShape* sharedShapePtr;
};

class Gizmo
{
public:
	RML::Transform transform;
public:
	Gizmo(const ROGLL::Mesh& mesh,
		Collider colliderA,
		Collider colliderB,
		Collider colliderC);
	~Gizmo();

	Axis GetAxisForIntersectedRay(const CppRayTracerChallenge::Core::Math::Ray& ray, RML::Vector& outHitPoint) const;

	Collider GetColliderX() const;
	Collider GetColliderY() const;
	Collider GetColliderZ() const;

	Gizmo(const Gizmo& other) = delete;
private:
	ROGLL::MeshInstance* m_meshInstance;
	Collider m_colliderX;
	Collider m_colliderY;
	Collider m_colliderZ;
};
