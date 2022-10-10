#pragma once

#include "RML.h"

enum class Axis
{
	NONE,
	X,
	Y,
	Z
};

RML::Vector AxisToDirection(Axis axis);
