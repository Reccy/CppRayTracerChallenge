#pragma once
#include "../math/i_shape.h"
#include "RML.h"

namespace CppRayTracerChallenge::Core::Renderer
{
	class IGroup
	{
	public:
		virtual const RML::Point worldToObject(RML::Point worldPosition) const = 0;
		virtual const RML::Vector normalToWorld(RML::Vector objectNormal) const = 0;
		virtual bool includes(const Math::IShape& other) const = 0;
	};
}
