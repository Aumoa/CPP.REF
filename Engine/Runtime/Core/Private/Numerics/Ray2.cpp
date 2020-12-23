// Copyright 2020 Aumoa.lib. All right reserved.

#include "Numerics/Ray2.h"

#include "HashHelper.h"
#include "CoreString.h"
#include "Numerics/Rectangle.h"

using namespace SC::Runtime::Core;
using namespace SC::Runtime::Core::Numerics;
using namespace std;

Ray2::Ray2()
{

}

Ray2::Ray2(const Vector2& origin, const Vector2& direction, optional<double> distance)
{
	Origin = origin;
	Direction = direction;
	Distance = distance;
}

bool Ray2::Equals(const Ray2& rh) const
{
	return Origin == rh.Origin
		&& Direction == rh.Direction
		&& Distance == rh.Distance;
}

bool Ray2::NearlyEquals(const Ray2& rh, double epsilon) const
{
	return Origin.NearlyEquals(rh.Origin, epsilon)
		&& Direction.NearlyEquals(rh.Direction, epsilon)
		&& (Distance.has_value() == rh.Distance.has_value() && abs(Distance.value() - rh.Distance.value()) <= epsilon);
}

size_t Ray2::GetHashCode() const
{
	return Origin.GetHashCode()
		 ^ Direction.GetHashCode()
		 ^ (Distance.has_value() ? HashHelper::GetHashCode(Distance.value()) : 0);
}

TRefPtr<String> Ray2::ToString() const
{
	TRefPtr<String> dist = Distance.has_value() ? String::Format("{0}", Distance.value()) : "Inf";
	return String::Format(
		"{{{0}: {1}, {2}: {3}, {4}: {5}}}",
		nameof(Origin),
		Origin,
		nameof(Direction),
		Direction,
		nameof(Distance),
		dist
	);
}

Vector2 Ray2::GetPoint(double distance) const
{
	return Origin + Direction * distance;
}

bool Ray2::IsOverlap(const Rectangle& rect) const
{
	return rect.IsOverlap(*this);
}

optional<double> Ray2::IsIntersect(const Rectangle& rect) const
{
	return rect.IsIntersect(*this);
}

bool Ray2::operator ==(const Ray2& rh) const
{
	return Origin == rh.Origin && Direction == rh.Direction && Distance == rh.Distance;
}

bool Ray2::operator !=(const Ray2& rh) const
{
	return Origin != rh.Origin || Direction != rh.Direction || Distance != rh.Distance;
}

bool Ray2::operator < (const Ray2& right) const
{
	const double dist = Distance.has_value() ? Distance.value() : -1;
	const double rdist = right.Distance.has_value() ? Distance.value() : -1;
	if (Origin < right.Origin)
	{
		return true;
	}
	else if (Origin > right.Origin)
	{
		return false;
	}
	else if (Direction < right.Direction)
	{
		return true;
	}
	else if (Direction > right.Direction)
	{
		return false;
	}
	else if (dist < rdist)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Ray2::operator <=(const Ray2& right) const
{
	const double dist = Distance.has_value() ? Distance.value() : -1;
	const double rdist = right.Distance.has_value() ? Distance.value() : -1;
	if (Origin < right.Origin)
	{
		return true;
	}
	else if (Origin > right.Origin)
	{
		return false;
	}
	else if (Direction < right.Direction)
	{
		return true;
	}
	else if (Direction > right.Direction)
	{
		return false;
	}
	else if (dist <= rdist)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Ray2::operator > (const Ray2& right) const
{
	const double dist = Distance.has_value() ? Distance.value() : -1;
	const double rdist = right.Distance.has_value() ? Distance.value() : -1;
	if (Origin > right.Origin)
	{
		return true;
	}
	else if (Origin < right.Origin)
	{
		return false;
	}
	else if (Direction > right.Direction)
	{
		return true;
	}
	else if (Direction < right.Direction)
	{
		return false;
	}
	else if (dist > rdist)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Ray2::operator >=(const Ray2& right) const
{
	const double dist = Distance.has_value() ? Distance.value() : -1;
	const double rdist = right.Distance.has_value() ? Distance.value() : -1;
	if (Origin > right.Origin)
	{
		return true;
	}
	else if (Origin < right.Origin)
	{
		return false;
	}
	else if (Direction > right.Direction)
	{
		return true;
	}
	else if (Direction < right.Direction)
	{
		return false;
	}
	else if (dist >= rdist)
	{
		return true;
	}
	else
	{
		return false;
	}
}

weak_ordering Ray2::operator <=>(const Ray2& right) const
{
	const double dist = Distance.has_value() ? Distance.value() : -1;
	const double rdist = right.Distance.has_value() ? Distance.value() : -1;
	if (Origin < right.Origin)
	{
		return weak_ordering::less;
	}
	else if (Origin > right.Origin)
	{
		return weak_ordering::greater;
	}
	else if (Direction < right.Direction)
	{
		return weak_ordering::less;
	}
	else if (Direction > right.Direction)
	{
		return weak_ordering::greater;
	}
	else if (dist < rdist)
	{
		return weak_ordering::less;
	}
	else if (dist > rdist)
	{
		return weak_ordering::greater;
	}
	else
	{
		return weak_ordering::equivalent;
	}
}