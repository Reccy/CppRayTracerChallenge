#include "world_serializer.h"

using namespace CppRayTracerChallenge::Core::Serializer;

void WorldSerializer::serialize()
{

}

void WorldSerializer::deserialize(std::vector<char> buffer)
{

}

CppRayTracerChallenge::Core::Renderer::World WorldSerializer::world() const
{
	return m_world;
}

CppRayTracerChallenge::Core::Renderer::Camera WorldSerializer::camera() const
{
	return m_camera;
}
