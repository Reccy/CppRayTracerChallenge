#ifndef _CPPRAYTRACERCHALLENGE_CORE_RENDERER_I_GROUP
#define _CPPRAYTRACERCHALLENGE_CORE_RENDERER_I_GROUP

#include "../math/point.h"
#include "../math/vector.h"

namespace CppRayTracerChallenge::Core::Renderer
{
	class IGroup
	{
	public:
		virtual const Math::Point worldToObject(Math::Point worldPosition) const = 0;
		virtual const Math::Vector normalToWorld(Math::Vector objectNormal) const = 0;
	};
}

#endif _CPPRAYTRACERCHALLENGE_CORE_RENDERER_I_GROUP