#include <gtest/gtest.h>
#include "math/trig.h"
#include "math/comparison.h"

using namespace CppRayTracerChallenge::Core::Math::Trig;
namespace Comparison = CppRayTracerChallenge::Core::Math::Comparison;

TEST(CppRayTracerChallenge_Core_Math_Trig, value_of_pi)
{
	EXPECT_EQ(PI, 3.141592653589793238462643383279502884);
}

TEST(CppRayTracerChallenge_Core_Math_Trig, convert_degrees_to_radians)
{
	double degrees = 45.0;
	double result = degrees_to_radians(degrees);

	double expectedResult = 0.785398;

	EXPECT_TRUE(Comparison::equal<double>(result, expectedResult));
}

TEST(CppRayTracerChallenge_Core_Math_Trig, convert_radians_to_degrees)
{
	double radians = 0.785398;
	double result = radians_to_degrees(radians);

	double expectedResult = 45.0;

	EXPECT_TRUE(Comparison::equal<double>(result, expectedResult));
}
