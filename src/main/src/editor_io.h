#pragma once

#include "graphics/image.h"
#include "serializer/base_image_serializer.h"
#include "editor_object.h"
#include "editor_material.h"
#include "editor_camera_settings.h"

namespace EditorIO
{
	using namespace CppRayTracerChallenge::Core;

	void WriteImage(Graphics::Image image, Serializer::BaseImageSerializer& serializer);
	void SaveWorld(const std::string& path, const CameraSettings& cameraSettings, const ROGLL::Camera& editorCamera, const std::vector<EditorObject*>& editorObjects, const std::vector<EditorMaterial*>& editorMaterials, const Graphics::Color& lightColor);
	void OpenWorld(const std::string& path, CameraSettings& cameraSettings, ROGLL::Camera& editorCamera, std::vector<EditorObject*>& editorObjects, std::vector<EditorMaterial*>& editorMaterials, Graphics::Color& lightColor);
}
