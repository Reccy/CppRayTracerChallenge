#include "editor_object.h"
#include "editor_core.h"

EditorObject::EditorObject() :
	id(0),
	name(""),
	eulerRotation(),
	transform(),
	objectType(),
	meshInstance(nullptr),
	material(&EditorCore::GetDefaultEditorMaterial())
{}

EditorObject::EditorObject(const EditorObject& other) :
	id(EditorDB::GenerateUniqueID()),
	name(other.name),
	eulerRotation(other.eulerRotation),
	transform(other.transform),
	objectType(other.objectType),
	material(other.material)
{
	meshInstance = new ROGLL::MeshInstance(*other.meshInstance);
}

EditorObject::~EditorObject()
{
	delete meshInstance;
}
