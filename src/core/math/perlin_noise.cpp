#include "perlin_noise.h"
#include "RML.h"
#include <cmath>
#include <string>

using namespace CppRayTracerChallenge::Core::Math;
using namespace RML;

// Permutation Table
const int P[] = {
	// original
	145,143,32,51,45,159,26,3,42,95,222,24,10,201,49,47,12,216,186,124,103,208,199,196,122,80,162,114,66,
	219,218,215,68,88,192,108,1,220,141,15,98,11,167,106,13,140,99,212,34,148,158,62,87,130,132,30,205,23,
	174,52,255,97,200,254,172,44,40,149,165,188,118,63,0,147,105,57,223,55,231,179,56,173,151,27,72,191,
	77,41,235,169,164,112,210,233,93,137,161,237,61,73,224,125,37,213,211,144,189,71,135,43,176,203,25,19,
	204,153,244,248,46,117,65,8,177,110,194,225,253,227,136,156,182,14,28,29,138,48,133,76,84,146,181,123,
	35,5,134,31,75,83,94,116,228,245,197,115,96,187,160,168,33,198,21,60,78,184,175,121,92,155,247,236,202,
	150,166,7,64,69,139,38,2,217,129,82,4,178,154,249,101,16,36,232,109,246,59,226,17,54,171,111,90,119,
	221,85,126,238,70,113,180,74,18,86,163,207,39,230,152,193,243,81,142,250,251,100,128,214,183,209,6,67,
	241,104,53,190,239,120,206,131,89,234,229,170,50,9,58,20,157,91,107,127,185,22,79,242,195,240,102,252,

	//copied
	145,143,32,51,45,159,26,3,42,95,222,24,10,201,49,47,12,216,186,124,103,208,199,196,122,80,162,114,66,
	219,218,215,68,88,192,108,1,220,141,15,98,11,167,106,13,140,99,212,34,148,158,62,87,130,132,30,205,23,
	174,52,255,97,200,254,172,44,40,149,165,188,118,63,0,147,105,57,223,55,231,179,56,173,151,27,72,191,
	77,41,235,169,164,112,210,233,93,137,161,237,61,73,224,125,37,213,211,144,189,71,135,43,176,203,25,19,
	204,153,244,248,46,117,65,8,177,110,194,225,253,227,136,156,182,14,28,29,138,48,133,76,84,146,181,123,
	35,5,134,31,75,83,94,116,228,245,197,115,96,187,160,168,33,198,21,60,78,184,175,121,92,155,247,236,202,
	150,166,7,64,69,139,38,2,217,129,82,4,178,154,249,101,16,36,232,109,246,59,226,17,54,171,111,90,119,
	221,85,126,238,70,113,180,74,18,86,163,207,39,230,152,193,243,81,142,250,251,100,128,214,183,209,6,67,
	241,104,53,190,239,120,206,131,89,234,229,170,50,9,58,20,157,91,107,127,185,22,79,242,195,240,102,252
};

int getPIndex(int x, int y, int z)
{
	return P[P[x] + y] + z;
}

Vector v(int index)
{
	int h = index & 15;

	if (h == 0)
		return Vector(1.0, 1.0, 0.0); // x + y
	else if (h == 1)
		return Vector(-1.0, 1.0, 0.0); // -x + y
	else if (h == 2)
		return Vector(1.0, -1.0, 0.0); // x - y
	else if (h == 3)
		return Vector(-1.0, -1.0, 0.0); // -x - y
	else if (h == 4)
		return Vector(1.0, 0.0, 1.0); // x + z
	else if (h == 5)
		return Vector(-1.0, 0.0, 1.0); // -x + z
	else if (h == 6)
		return Vector(1.0, 0.0, -1.0); // x - z
	else if (h == 7)
		return Vector(-1.0, 0.0, -1.0); // -x - z
	else if (h == 8)
		return Vector(0.0, 1.0, 1.0); // y + z
	else if (h == 9)
		return Vector(0.0, -1.0, 1.0); // -y + z
	else if (h == 10)
		return Vector(0.0, 1.0, -1.0); // y - z
	else if (h == 11)
		return Vector(0.0, -1.0, -1.0); // -y - z
	else if (h == 12)
		return Vector(1.0, 1.0, 0.0); // y + x
	else if (h == 13)
		return Vector(0.0, -1.0, 1.0); // -y + z
	else if (h == 14)
		return Vector(-1.0, 1.0, 0.0); // y - x
	else
		return Vector(0.0, -1.0, -1.0); // -y - z
}

double fade(double t)
{
	return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
}

double lerp(double t, double a, double b)
{
	return a + t * (b - a);
}

PerlinNoise::PerlinNoise() {};

double PerlinNoise::at(Point position) const
{
	// Input values
	double inX = position.x();
	double inY = position.y();
	double inZ = position.z();

	// Indices for each cube position
	int cubeX = static_cast<int>(floor(inX)) & 255;
	int cubeY = static_cast<int>(floor(inY)) & 255;
	int cubeZ = static_cast<int>(floor(inZ)) & 255;

	// Fractional co-ordinates inside each cube
	double fracX = inX - floor(inX);
	double fracY = inY - floor(inY);
	double fracZ = inZ - floor(inZ);

	// Setup Vectors on each integer intersection
	double const V_FORWARD = fracZ - 1.0;
	double const V_BACKWARD = fracZ;
	double const V_TOP = fracY - 1.0;
	double const V_BOTTOM = fracY;
	double const V_RIGHT = fracX - 1.0;
	double const V_LEFT = fracX;

	Vector leftTopForwardToPos = Vector(V_LEFT, V_TOP, V_FORWARD);
	Vector leftTopBackwardToPos = Vector(V_LEFT, V_TOP, V_BACKWARD);
	Vector leftBottomForwardToPos = Vector(V_LEFT, V_BOTTOM, V_FORWARD);
	Vector leftBottomBackwardToPos = Vector(V_LEFT, V_BOTTOM, V_BACKWARD);
	Vector rightTopForwardToPos = Vector(V_RIGHT, V_TOP, V_FORWARD);
	Vector rightTopBackwardToPos = Vector(V_RIGHT, V_TOP, V_BACKWARD);
	Vector rightBottomForwardToPos = Vector(V_RIGHT, V_BOTTOM, V_FORWARD);
	Vector rightBottomBackwardToPos = Vector(V_RIGHT, V_BOTTOM, V_BACKWARD);

	int const P_FORWARD = cubeZ + 1;
	int const P_BACKWARD = cubeZ;
	int const P_TOP = cubeY + 1;
	int const P_BOTTOM = cubeY;
	int const P_RIGHT = cubeX + 1;
	int const P_LEFT = cubeX;

	int leftTopForwardIndex = getPIndex(P_LEFT, P_TOP, P_FORWARD);
	int leftTopBackwardIndex = getPIndex(P_LEFT, P_TOP, P_BACKWARD);
	int leftBottomForwardIndex = getPIndex(P_LEFT, P_BOTTOM, P_FORWARD);
	int leftBottomBackwardIndex = getPIndex(P_LEFT, P_BOTTOM, P_BACKWARD);
	int rightTopForwardIndex = getPIndex(P_RIGHT, P_TOP, P_FORWARD);
	int rightTopBackwardIndex = getPIndex(P_RIGHT, P_TOP, P_BACKWARD);
	int rightBottomForwardIndex = getPIndex(P_RIGHT, P_BOTTOM, P_FORWARD);
	int rightBottomBackwardIndex = getPIndex(P_RIGHT, P_BOTTOM, P_BACKWARD);

	double dotLeftTopForward = Vector::dot(leftTopForwardToPos, v(leftTopForwardIndex));
	double dotLeftTopBackward = Vector::dot(leftTopBackwardToPos, v(leftTopBackwardIndex));
	double dotLeftBottomForward = Vector::dot(leftBottomForwardToPos, v(leftBottomForwardIndex));
	double dotLeftBottomBackward = Vector::dot(leftBottomBackwardToPos, v(leftBottomBackwardIndex));
	double dotRightTopForward = Vector::dot(rightTopForwardToPos, v(rightTopForwardIndex));
	double dotRightTopBackward = Vector::dot(rightTopBackwardToPos, v(rightTopBackwardIndex));
	double dotRightBottomForward = Vector::dot(rightBottomForwardToPos, v(rightBottomForwardIndex));
	double dotRightBottomBackward = Vector::dot(rightBottomBackwardToPos, v(rightBottomBackwardIndex));

	double u = fade(fracX);
	double v = fade(fracY);
	double w = fade(fracZ);

	double forward = lerp(u,
		lerp(v,
			dotLeftBottomForward,
			dotLeftTopForward),
		lerp(v,
			dotRightBottomForward,
			dotRightTopForward));

	double backward = lerp(u,
		lerp(v,
			dotLeftBottomBackward,
			dotLeftTopBackward),
		lerp(v,
			dotRightBottomBackward,
			dotRightTopBackward));

	double result = lerp(w, backward, forward);

	result += 1;
	result /= 2;
	return result;
}
