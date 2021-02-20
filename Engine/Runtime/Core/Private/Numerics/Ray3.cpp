// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Numerics/Ray3.h"

#include "HashHelper.h"
#include "CoreString.h"
#include "Numerics/AxisAlignedCube.h"

using namespace std;

Ray3::Ray3()
{

}

Ray3::Ray3(const Vector3& origin, const Vector3& direction, optional<float> distance)
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

bool Ray3::NearlyEquals(const Ray3& rh, float epsilon) const
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

Vector3 Ray3::GetPoint(float distance) const
{
	return Origin + Direction * distance;
}

Vector3 Ray3::GetEndPoint() const
{
	if (Distance.has_value())
	{
		return GetPoint(Distance.value());
	}
	else
	{
		return Direction * std::numeric_limits<float>::infinity();
	}
}