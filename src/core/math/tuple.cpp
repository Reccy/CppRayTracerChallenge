#include "tuple.h"

float CppRayTracerChallenge::Core::Math::Tuple::getX()
{
	return this->m_x;
}

float CppRayTracerChallenge::Core::Math::Tuple::getY()
{
	return this->m_y;
}

float CppRayTracerChallenge::Core::Math::Tuple::getZ()
{
	return this->m_z;
}

float CppRayTracerChallenge::Core::Math::Tuple::getW()
{
	return this->m_w;
}

bool CppRayTracerChallenge::Core::Math::Tuple::isPoint()
{
	return this->m_w == 1.0f;
}

bool CppRayTracerChallenge::Core::Math::Tuple::isVector()
{
	return this->m_w == 0.0f;
}
