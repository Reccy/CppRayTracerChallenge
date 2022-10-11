#pragma once

#include "rogll/include.h"
#include "editor_material.h"
#include "editor_axis.h"
#include "editor_db.h"

struct EditorObject
{
	enum Type
	{
		CUBE,
		PLANE,
		LIGHT,
		SPHERE,
		CYLINDER
	};

	unsigned int id;
	std::string name;
	RML::Transform transform;
	RML::Vector eulerRotation;
	EditorObject::Type objectType;
	ROGLL::MeshInstance* meshInstance;
	const EditorMaterial* material;

	EditorObject();
	EditorObject(const EditorObject& other);
	~EditorObject();
	
	static ROGLL::MeshInstance* CreateMeshInstanceForType(EditorObject::Type type);
	static RML::Vector AxisToLocalDirection(const EditorObject& object, Axis axis);
	static std::string GetTypeName(EditorObject::Type objectType);
};
