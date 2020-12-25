// Copyright 2020 Aumoa.lib. All right reserved.

#include "Numerics/Ray3.h"

#include "HashHelper.h"
#include "CoreString.h"
#include "Numerics/AxisAlignedCube.h"

using namespace SC::Runtime::Core;
using namespace SC::Runtime::Core::Numerics;
using namespace std;

Ray3::Ray3()
{

}

Ray3::Ray3(const Vector3& origin, const Vector3& direction, optional<double> distance)
{
	Origin = origin;
	Direction = direction;
	Distance = distance;
}

Ray3::Ray3(const Ray3& copy)
{
	Origin = copy.Origin;
	Direction = copy.Direction;
	Distance = copy.Distance;
}

bool Ray3::Equals(const Ray3& rh) const
{
	return Origin == rh.Origin
		&& Direction == rh.Direction
		&& Distance == rh.Distance;
}

bool Ray3::NearlyEquals(const Ray3& rh, double epsilon) const
{
	return Origin.NearlyEquals(rh.Origin, epsilon)
		&& Direction.NearlyEquals(rh.Direction, epsilon)
		&& (Distance.has_value() == rh.Distance.has_value() && abs(Distance.value() - rh.Distance.value()) <= epsilon);
}

size_t Ray3::GetHashCode() const
{
	return Origin.GetHashCode()
		^ Direction.GetHashCode()
		^ (Distance.has_value() ? HashHelper::GetHashCode(Distance.value()) : 0);
}

TRefPtr<String> Ray3::ToString() const
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

Vector3 Ray3::GetPoint(double distance) const
{
	return Origin + Direction * distance;
}

bool Ray3::IsOverlap(const AxisAlignedCube& cube) const
{
	return cube.IsOverlap(*this);
}

optional<double> Ray3::IsIntersect(const AxisAlignedCube& cube) const
{
	return cube.IsIntersect(*this);
}

bool Ray3::operator ==(const Ray3& rh) const
{
	return Origin == rh.Origin && Direction == rh.Direction && Distance == rh.Distance;
}

bool Ray3::operator !=(const Ray3& rh) const
{
	return Origin != rh.Origin || Direction != rh.Direction || Distance != rh.Distance;
}

bool Ray3::operator < (const Ray3& right) const
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

bool Ray3::operator <=(const Ray3& right) const
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

bool Ray3::operator > (const Ray3& right) const
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

bool Ray3::operator >=(const Ray3& right) const
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

weak_ordering Ray3::operator <=>(const Ray3& right) const
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