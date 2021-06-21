#include "material.h"

using namespace CppRayTracerChallenge::Core::Renderer;

bool ::CppRayTracerChallenge::Core::Renderer::operator==(const Material& lhs, const Material& rhs)
{
	if (*lhs.pattern != *rhs.pattern) return false;

	if (!Math::Comparison::equal(lhs.ambient, rhs.ambient)) return false;

	if (!Math::Comparison::equal(lhs.diffuse, rhs.diffuse)) return false;

	if (!Math::Comparison::equal(lhs.specular, rhs.specular)) return false;

	if (!Math::Comparison::equal(lhs.shininess, rhs.shininess)) return false;

	return true;
}

bool ::CppRayTracerChallenge::Core::Renderer::operator!=(const Material& lhs, const Material& rhs)
{
	return !(lhs == rhs);
}

std::ostream& CppRayTracerChallenge::Core::Renderer::operator<<(std::ostream& os, const Material& material)
{
	os << "&P[" << material.pattern << "] - ";
	os << "A[" << material.ambient << "] - ";
	os << "D[" << material.diffuse << "] - ";
	os << "Sp[" << material.specular << "] - ";
	os << "Sh[" << material.shininess << "]" << std::endl;
	return os;
}
