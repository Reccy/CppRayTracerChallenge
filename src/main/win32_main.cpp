#include <iostream>
#include "math/tuple.h"

using namespace CppRayTracerChallenge::Core;

int main()
{
	Math::Tuple t = Math::Tuple::buildPoint(1, 2, 3);
	std::cout << t << std::endl;
	return 0;
}
