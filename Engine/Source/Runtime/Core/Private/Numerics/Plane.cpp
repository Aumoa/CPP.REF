// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Numerics/Plane.h"

#include "HashHelper.h"
#include "Core/String.h"
#include "Mathematics/Math.h"

Plane::Plane()
{

}

Plane::Plane(const Vector3& inNormal, float inDistance)
{
	Normal = inNormal;
	Distance = inDistance;
}

bool Plane::Equals(const Plane& rh) const
{
	return *this == rh;
}

bool Plane::NearlyEquals(const Plane& rh, float epsilon) const
{
	return Normal.NearlyEquals(rh.Normal, epsilon)
		&& Math::Abs(Distance - rh.Distance) <= epsilon;
}

size_t Plane::GetHashCode() const
{
	return Normal.GetHashCode() ^ HashHelper::GetHashCode(Distance);
}

TRefPtr<String> Plane::ToString() const
{
	return String::Format(L"Normal: {0}, Dist: {1}", Normal, Distance);
}

float Plane::GetComponentOrDefault(size_t index) const
{
	if (index < Count)
	{
		return operator [](index);
	}
	else
	{
		return 0;
	}
}

bool Plane::Contains(size_t index) const
{
	return index < Count;
}

Plane Plane::Normalized_get() const
{
	float sq = Normal.LengthSq;
	float rt = Math::InvSqrt(sq);

	return Plane(Normal * rt, Distance * rt);
}

const float& Plane::operator [](size_t index) const
{
	const float* ptr = &Normal.X;
	return ptr[index];
}

float& Plane::operator [](size_t index)
{
	float* ptr = &Normal.X;
	return ptr[index];
}

float Plane::DotCoord(const Plane& inPlane, const Vector3& inVector)
{
	return inPlane[0] * inVector[0] + inPlane[1] * inVector[1] + inPlane[2] * inVector[2] + inPlane[3] * 1.0f;
}