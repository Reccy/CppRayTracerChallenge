#include "world_deserializer.h"

#include <map>
#include <ryml_std.hpp>
#include <ryml.hpp>

#include "../math/cube.h"

using namespace CppRayTracerChallenge::Core;

std::string toStr(c4::yml::NodeRef node)
{
	return std::string(node.val().begin(), node.val().end());
}

int toInt(c4::yml::NodeRef node)
{
	std::string str = toStr(node);
	return std::stoi(toStr(node));
}

float toFloat(c4::yml::NodeRef node)
{
	return std::stof(toStr(node));
}

Graphics::Color toColor(c4::yml::NodeRef color)
{
	auto r = toFloat(color["r"]);
	auto g = toFloat(color["g"]);
	auto b = toFloat(color["b"]);

	return Graphics::Color(r, g, b);
}

Math::Transform toTransform(c4::yml::NodeRef transformNode)
{
	float translateX = toFloat(transformNode["translate"]["x"]);
	float translateY = toFloat(transformNode["translate"]["y"]);
	float translateZ = toFloat(transformNode["translate"]["z"]);

	float rotateX = toFloat(transformNode["rotate"]["x"]);
	float rotateY = toFloat(transformNode["rotate"]["y"]);
	float rotateZ = toFloat(transformNode["rotate"]["z"]);

	float scaleX = toFloat(transformNode["scale"]["x"]);
	float scaleY = toFloat(transformNode["scale"]["y"]);
	float scaleZ = toFloat(transformNode["scale"]["z"]);

	return Math::Transform()
		.translate(translateX, translateY, translateZ)
		.rotate(rotateX, rotateY, rotateZ)
		.scale(scaleX, scaleY, scaleZ);
}

Math::Vector toVec3(c4::yml::NodeRef node)
{
	float x = toFloat(node["x"]);
	float y = toFloat(node["y"]);
	float z = toFloat(node["z"]);

	return Math::Vector(x, y, z);
}

std::shared_ptr<Renderer::Pattern> buildPattern(c4::yml::NodeRef pattern)
{
	std::string type = toStr(pattern["type"]);

	if (type != "solid") // todo: fix quick and dirty pattern type check here
	{
		std::cerr << "Invalid Pattern type: " << type << std::endl;
		abort();
	}

	Graphics::Color color = toColor(pattern["color"]);

	return std::make_shared<Renderer::Patterns::SolidColor>(Renderer::Patterns::SolidColor(color));
}

Serializer::WorldDeserializer::WorldDeserializer(const std::string buffer)
{
	ryml::Tree root = ryml::parse_in_arena(ryml::to_csubstr(buffer));

	// Check Version Match
	int version = toInt(root["version"]);

	if (version != 1) // todo: quick and dirty version check - refactor later
	{
		std::cerr << "Cannot read file: Version mismatch" << std::endl;
		abort();
	}

	// Read Materials
	ryml::NodeRef materialListRoot = root["materials"];

	std::map<int, Renderer::Material> materialsMap;

	for (auto it = materialListRoot.children().begin(); it != materialListRoot.children().end(); ++it)
	{
		ryml::NodeRef materialRoot = *it;

		Renderer::Material material;
		material.pattern = buildPattern(materialRoot["pattern"]);
		material.reflective = toFloat(materialRoot["reflective"]);
		material.specular = toFloat(materialRoot["specular"]);
		material.diffuse = toFloat(materialRoot["diffuse"]);

		int id = toInt(materialRoot["uuid"]);
		materialsMap[id] = material;
	}

	// Read Hierarchy Start
	ryml::NodeRef hierarchy = root["hierarchy"];

	// Read Objects
	ryml::NodeRef objectListRoot = hierarchy["objects"];

	std::map<int, Renderer::Shape> objectsMap;

	for (auto it = objectListRoot.children().begin(); it != objectListRoot.children().end(); ++it)
	{
		ryml::NodeRef objectRoot = *it;

		std::string shapeType = toStr(objectRoot["shape"]);
		if (shapeType != "cube") // todo: fix another quick dirty bit of code - dirty shape check
		{
			std::cerr << "Invalid Shape type: " << shapeType << std::endl;
			abort();
		}

		auto shape = std::make_shared<Math::Cube>();
		Renderer::Shape object = Renderer::Shape(shape);

		int materialId = toInt(objectRoot["material"]);

		object.material(materialsMap[materialId]);
		object.transform(toTransform(objectRoot["transform"]));

		int id = toInt(objectRoot["uuid"]);
		objectsMap[id] = object;
	}

	// Read Lights
	ryml::NodeRef lightListRoot = hierarchy["lights"];

	std::map<int, Renderer::PointLight> lightsMap;

	for (auto it = lightListRoot.children().begin(); it != lightListRoot.children().end(); ++it)
	{
		ryml::NodeRef lightRoot = *it;

		std::string lightType = toStr(lightRoot["light_type"]);
		if (lightType != "point") // todo: fix quick and dirty bit again
		{
			std::cerr << "Invalid Light type: " << lightType << std::endl;
			abort();
		}

		Graphics::Color intensity = toColor(lightRoot["intensity"]);
		Math::Vector position = toVec3(lightRoot["transform"]);
		
		Renderer::PointLight light = Renderer::PointLight(position, intensity);

		int id = toInt(lightRoot["uuid"]);
		lightsMap[id] = light;
	}

	// Create World, Configure Settings
	m_world = std::make_shared<Renderer::World>();

	m_world->defaultRemainingCalls = toInt(root["settings"]["default_remaining_calls"]); // todo: move to its own settings function

	// Add lights
	for (auto it = lightsMap.begin(); it != lightsMap.end(); ++it)
	{
		auto light = (*it).second;
		m_world->addLight(light);
	}

	// Add Objects
	for (auto it = objectsMap.begin(); it != objectsMap.end(); ++it)
	{
		auto object = (*it).second;
		auto objectPtr = std::make_shared<Renderer::Shape>(object);
		m_world->addObject(objectPtr);
	}
}

const std::shared_ptr<Renderer::World> Serializer::WorldDeserializer::world() const
{
	return m_world;
}