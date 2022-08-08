#pragma once
#include "RML.h"

namespace CppRayTracerChallenge::Core::Math
{
	/// <summary>
	/// Perlin Noise generator. Each instance has its own seed.
	/// </summary>
	class PerlinNoise
	{
	public:
		/// <summary>
		/// Creates an instance with default seed
		/// </summary>
		PerlinNoise();

		/// <summary>
		/// Gets the value of the noise at the passed in point
		/// </summary>
		/// <param name="position">The point at where to evaluate the noise</param>
		/// <returns>A generated value between 0 and 1</returns>
		double at(RML::Point position) const;
	};
}
