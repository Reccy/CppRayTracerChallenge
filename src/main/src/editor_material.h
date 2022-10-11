#pragma once

#include "rogll/include.h"
#include "renderer/material.h"
#include "editor_pattern.h"

using namespace CppRayTracerChallenge::Core;

struct EditorMaterial
{
	bool isProtected = false;
	unsigned int id;
	std::string name;
	float ambient = 1;
	float diffuse = 0;
	float specular = 0;
	float shininess = 0;
	float reflective = 0;
	float transparency = 0;
	float refractiveIndex = 0;

	Renderer::ShadowcastMode shadowcastMode = Renderer::ShadowcastMode::WHEN_TRANSPARENT;

	EditorPatternBase* editorPattern;

	ROGLL::Material* previewMaterial;
	ROGLL::RenderBatch* renderBatch;

	EditorMaterial();
	EditorMaterial(EditorPatternBase* editorPattern);
	EditorMaterial(const EditorMaterial& other);
	~EditorMaterial();

	std::shared_ptr<Renderer::Pattern> BuildPattern() const;
	void DrawPatternProperties();
	Renderer::Material CreateRendererMaterial() const;

	static EditorMaterial* CreateInContainer(std::vector<EditorMaterial*>& container);
};
