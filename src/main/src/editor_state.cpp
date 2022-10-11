#include "editor_state.h"

EditorState::EditorState() :
	lightColor(1, 1, 1),
	editorCamera(120, 120, 60),
	selectedObject(nullptr),
	selectedMaterial(nullptr),
	selectedObjectHitAxis(Axis::NONE),
	cameraSettings({60, 0, 0, 1920, 1080})
{}

static RML::Tuple3<float> ColorToTuple(const Graphics::Color& c)
{
	return RML::Tuple3<float> { c.red(), c.green(), c.blue() };
}

static Graphics::Color TupleToColor(const RML::Tuple3<float>& tuple)
{
	return Graphics::Color(tuple.x(), tuple.y(), tuple.z());
}

RML::Tuple3<float> EditorState::GetLightColorTuple() const
{
	return ColorToTuple(lightColor);
}
