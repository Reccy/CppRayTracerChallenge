#include "camera_deserializer.h"

#include <ryml_std.hpp>
#include <ryml.hpp>

using namespace CppRayTracerChallenge::Core;

// todo: merge duplicated code from here with the world deserializer

namespace
{
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

	Math::Vector toVec3(c4::yml::NodeRef node)
	{
		float x = toFloat(node["x"]);
		float y = toFloat(node["y"]);
		float z = toFloat(node["z"]);

		return Math::Vector(x, y, z);
	}
}

Serializer::CameraDeserializer::CameraDeserializer(const std::string buffer)
{
	ryml::Tree root = ryml::parse_in_arena(ryml::to_csubstr(buffer));

	// Check Version Match
	int version = toInt(root["version"]);

	if (version != 1) // todo: quick and dirty version check - refactor later
	{
		std::cerr << "Cannot read file: Version mismatch" << std::endl;
		abort();
	}

	ryml::NodeRef cameraRoot = root["camera"];

	auto width = toInt(cameraRoot["resolution"]["width"]);
	auto height = toInt(cameraRoot["resolution"]["height"]);

	auto fov = toInt(cameraRoot["fov"]);

	auto camOrigin = toVec3(cameraRoot["origin"]);
	auto camTarget = toVec3(cameraRoot["target"]);
	auto camUp = toVec3(cameraRoot["up"]);

	auto camMatrix = m_camera->viewMatrix(camOrigin, camTarget, camUp);
	
	m_camera = std::make_shared<Renderer::Camera>(width, height, fov);
	m_camera->transform(camMatrix);
}

const std::shared_ptr<Renderer::Camera> Serializer::CameraDeserializer::camera() const {
	return m_camera;
}
