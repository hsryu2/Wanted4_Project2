#include "Bounds.h"

namespace Wanted
{
	bool Bounds::Intersects(const Bounds& other) const
	{
		if (other.x > MaxX())
		{
			return false;
		}

		if (other.MaxX() < x)
		{
			return false;
		}

		if (other.y > MaxX())
		{
			return false;
		}

		if (other.MaxY() < y)
		{
			return false;
		}

		return true;
	}

}

