// Copyright 2020 Aumoa.lib. All right reserved.

#include "Numerics/AxisAlignedCube.h"

#include "CoreString.h"
#include "Numerics/Ray3.h"

using namespace std;

AxisAlignedCube::AxisAlignedCube()
{

}

AxisAlignedCube::AxisAlignedCube(const Vector3& min, const Vector3& max)
	: Min(min)
	, Max(max)
{
	
}

AxisAlignedCube::AxisAlignedCube(const AxisAlignedCube& copy)
	: Min(copy.Min)
	, Max(copy.Max)
{

}

bool AxisAlignedCube::Equals(const AxisAlignedCube& rh) const
{
	return Min == rh.Min
		&& Max == rh.Max;
}

bool AxisAlignedCube::NearlyEquals(const AxisAlignedCube& rh, float epsilon) const
{
	return Min.NearlyEquals(rh.Min, epsilon)
		&& Max.NearlyEquals(rh.Max, epsilon);
}

size_t AxisAlignedCube::GetHashCode() const
{
	return Min.GetHashCode() ^ Max.GetHashCode();
}

TRefPtr<String> AxisAlignedCube::ToString() const
{
	return String::Format("{{Min: {0}, Max: {1}}}", Min, Max);
}

Vector3 AxisAlignedCube::GetPoint(size_t index) const
{
	const Vector3* vp[] = { &Min, &Max };
	return Vector3(vp[index / 4]->X, vp[(index % 4) / 2]->Y, vp[index % 2]->Z);
}

Vector3 AxisAlignedCube::Center_get() const
{
	return (Max - Min) * 0.5f;
}