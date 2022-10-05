#pragma once

#include "rogll/include.h"
#include "editor_material.h"
#include "editor_db.h"

enum class EditorObjectType
{
	CUBE,
	PLANE,
	LIGHT,
	SPHERE,
	CYLINDER
};

struct EditorObject
{
	unsigned int id;
	std::string name;
	RML::Transform transform;
	RML::Vector eulerRotation;
	EditorObjectType objectType;
	ROGLL::MeshInstance* meshInstance;
	const EditorMaterial* material;

	EditorObject();
	EditorObject(const EditorObject& other);
	~EditorObject();
};
