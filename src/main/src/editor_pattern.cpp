#include "editor_pattern.h"

#include <tinyxml2.h>
#include <imgui.h>
#include "renderer/pattern.h"
#include "renderer/patterns/solid_color.h"

using namespace CppRayTracerChallenge::Core;

std::shared_ptr<Renderer::Pattern> EditorPatternSolidColor::BuildPattern() const
{
	auto result = std::make_shared<Renderer::Patterns::SolidColor>(color);
	return result;
}

EditorPatternBase* EditorPatternSolidColor::Clone() const
{
	EditorPatternSolidColor* result = new EditorPatternSolidColor();

	result->color = color;

	return result;
}

void EditorPatternSolidColor::Serialize(tinyxml2::XMLPrinter& printer) const
{
	printer.PushAttribute("type", "SOLID_COLOR");
	printer.OpenElement("color");
	printer.PushAttribute("r", color.red());
	printer.PushAttribute("g", color.green());
	printer.PushAttribute("b", color.blue());
	printer.CloseElement();
}

void EditorPatternSolidColor::DrawProperties()
{
	float fColor[3] = { color.red(), color.green(), color.blue() };
	ImGui::ColorPicker3("Color", fColor);
	color = Graphics::Color(fColor[0], fColor[1], fColor[2]);
}
