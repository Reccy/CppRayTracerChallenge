#pragma once

#include <vector>

#include "rogll/include.h"
#include "graphics/color.h"
#include "editor_object.h"

namespace EditorCore
{
	typedef unsigned char byte;

	void StartFullRender(int width, int height, float fov, ROGLL::Camera& editorCamera, const std::vector<EditorObject*>& editorObjects, Graphics::Color lightColor);
	byte* GetRenderPixels();

	bool IsRenderInProgress();

	const ROGLL::Shader& GetDefaultShader();
	const ROGLL::VertexAttributes& GetDefaultLayout();
	void UpdateDefaultEditorMaterial(EditorMaterial* newEditorMaterial);
	const EditorMaterial& GetDefaultEditorMaterial();
}
