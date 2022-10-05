#pragma once

#include "graphics/image.h"
#include "serializer/base_image_serializer.h"

namespace EditorIO
{
	using namespace CppRayTracerChallenge::Core;

	void WriteImage(Graphics::Image image, Serializer::BaseImageSerializer& serializer);
}
