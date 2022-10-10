#include "editor_axis.h"

RML::Vector AxisToDirection(Axis axis)
{
	switch (axis)
	{
	case Axis::X: return RML::Vector::right();
	case Axis::Y: return RML::Vector::up();
	case Axis::Z: return RML::Vector::forward();
	}
}
