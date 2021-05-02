#include "intersection.h"

using namespace CppRayTracerChallenge::Core::Math;

Intersection::Intersection(const double t, const IIntersectable& intersectable) : m_t(t), m_intersectable(intersectable) {};
