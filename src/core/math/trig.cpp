#include "trig.h"

using namespace CppRayTracerChallenge::Core::Math;

double Trig::degrees_to_radians(const double degrees)
{
	return degrees * (PI / 180.0);
};

double Trig::radians_to_degrees(const double radians)
{
	return (radians / PI) * 180.0;
};
