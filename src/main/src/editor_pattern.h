#pragma once

#include <tinyxml2.h>
#include "renderer/pattern.h"

using namespace CppRayTracerChallenge::Core;

class EditorPatternBase
{
public:
	virtual void DrawProperties() = 0;
	virtual std::shared_ptr<Renderer::Pattern> BuildPattern() const = 0;
	virtual EditorPatternBase* Clone() const = 0;
	virtual void Serialize(tinyxml2::XMLPrinter& printer) const = 0;
	virtual ~EditorPatternBase() {};
};

class EditorPatternSolidColor : public EditorPatternBase
{
public:
	Graphics::Color color = Graphics::Color::white();
	std::shared_ptr<Renderer::Pattern> BuildPattern() const;
	EditorPatternBase* Clone() const override;
	void Serialize(tinyxml2::XMLPrinter& printer) const override;
	void DrawProperties() override;
};
