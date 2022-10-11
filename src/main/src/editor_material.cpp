#include "editor_material.h"

#include <string>

#include "editor_core.h"
#include "rogll/include.h"
#include "renderer/camera.h"

EditorMaterial::EditorMaterial() :
	isProtected(false),
	id(0),
	name(""),
	ambient(1),
	diffuse(0),
	specular(0),
	shininess(0),
	reflective(0),
	transparency(0),
	refractiveIndex(0),
	shadowcastMode(Renderer::ShadowcastMode::WHEN_TRANSPARENT)
{
	editorPattern = new EditorPatternSolidColor();
	previewMaterial = new ROGLL::Material(EditorCore::GetDefaultShader()); // TODO: Support multiple patterns
	renderBatch = new ROGLL::RenderBatch(&EditorCore::GetDefaultLayout(), previewMaterial);
}

EditorMaterial::EditorMaterial(EditorPatternBase* editorPattern) :
	isProtected(false),
	id(0),
	name(""),
	ambient(1),
	diffuse(0),
	specular(0),
	shininess(0),
	reflective(0),
	transparency(0),
	refractiveIndex(0),
	shadowcastMode(Renderer::ShadowcastMode::WHEN_TRANSPARENT)
{
	this->editorPattern = editorPattern;
	previewMaterial = new ROGLL::Material(EditorCore::GetDefaultShader()); // TODO: Support multiple patterns
	renderBatch = new ROGLL::RenderBatch(&EditorCore::GetDefaultLayout(), previewMaterial);
}

EditorMaterial::EditorMaterial(const EditorMaterial& other) :
	isProtected(false),
	id(0),
	name(other.name),
	ambient(other.ambient),
	diffuse(other.diffuse),
	specular(other.specular),
	shininess(other.shininess),
	reflective(other.reflective),
	transparency(other.transparency),
	refractiveIndex(other.refractiveIndex),
	shadowcastMode(other.shadowcastMode)
{
	editorPattern = other.editorPattern->Clone();
	previewMaterial = new ROGLL::Material(*other.previewMaterial);
	renderBatch = new ROGLL::RenderBatch(&EditorCore::GetDefaultLayout(), previewMaterial);
}

EditorMaterial::~EditorMaterial()
{
	delete editorPattern;
	delete previewMaterial;
	delete renderBatch;
}

std::shared_ptr<Renderer::Pattern> EditorMaterial::BuildPattern() const
{
	return editorPattern->BuildPattern();
}

void EditorMaterial::DrawPatternProperties()
{
	editorPattern->DrawProperties();
}

Renderer::Material EditorMaterial::CreateRendererMaterial() const
{
	Renderer::Material result;

	result.ambient = ambient;
	result.diffuse = diffuse;
	result.specular = specular;
	result.shininess = shininess;
	result.reflective = reflective;
	result.transparency = transparency;
	result.refractiveIndex = refractiveIndex;
	result.shadowcastMode = shadowcastMode;

	result.pattern = BuildPattern();

	return result;
}

EditorMaterial* EditorMaterial::CreateInContainer(std::vector<EditorMaterial*>& container)
{
	EditorMaterial* mat = new EditorMaterial();
	mat->id = EditorDB::GenerateUniqueID();
	mat->name = "New Material";
	container.push_back(mat);
	return container[container.size() - 1];
}
