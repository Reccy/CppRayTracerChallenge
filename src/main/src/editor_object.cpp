#include "editor_object.h"
#include "editor_core.h"

#include <assert.h>

extern ROGLL::Mesh* CubeMesh;
extern ROGLL::Mesh* CylinderMesh;
extern ROGLL::Mesh* LightMesh;
extern ROGLL::Mesh* PlaneMesh;
extern ROGLL::Mesh* SphereMesh;

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

ROGLL::MeshInstance* EditorObject::CreateMeshInstanceForType(EditorObject::Type type)
{
	if (type == EditorObject::Type::CUBE) return new ROGLL::MeshInstance(*CubeMesh);
	if (type == EditorObject::Type::CYLINDER) return new ROGLL::MeshInstance(*CylinderMesh);
	if (type == EditorObject::Type::LIGHT) return new ROGLL::MeshInstance(*LightMesh);
	if (type == EditorObject::Type::PLANE) return new ROGLL::MeshInstance(*PlaneMesh);
	if (type == EditorObject::Type::SPHERE) return new ROGLL::MeshInstance(*SphereMesh);

	assert(false);

	return nullptr;
}
