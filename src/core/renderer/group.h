#ifndef _CPPRAYTRACERCHALLENGE_CORE_RENDERER_GROUP
#define _CPPRAYTRACERCHALLENGE_CORE_RENDERER_GROUP

#include "../math/i_shape.h"
#include "shape.h"

namespace CppRayTracerChallenge::Core::Renderer
{
	using namespace CppRayTracerChallenge::Core::Math;

	class Group : public Math::IShape
	{
	public:
		void transform(Transform transform) override;

		const Transform transform() const override;

		const Vector normal(const Point position) const override;

		const Vector normalLocal(const Point position) const override;

		const Intersections intersect(Ray ray) const override;

		const Intersections intersectLocal(Ray ray) const override;

		int size() const;

		int count() const;

		bool empty() const;
	private:
		Transform m_transform;
		std::vector<std::unique_ptr<Renderer::Shape>> m_shapes;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_RENDERER_GROUP
