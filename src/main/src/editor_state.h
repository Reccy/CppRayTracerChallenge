#pragma once

#include "editor_object.h"
#include "editor_material.h"
#include "graphics/color.h"
#include "editor_object.h"
#include "editor_material.h"
#include "editor_axis.h"
#include "editor_camera_settings.h"

struct EditorState
{
	std::vector<EditorObject*> editorObjects;
	std::vector<EditorMaterial*> editorMaterials;

	Graphics::Color lightColor;
	RML::Tuple3<float> lightPosition;

	ROGLL::Camera editorCamera;

	EditorObject* selectedObject;
	EditorMaterial* selectedMaterial;
	Axis selectedObjectHitAxis;

	CameraSettings cameraSettings;

	EditorState();

	RML::Tuple3<float> GetLightColorTuple() const;
};
