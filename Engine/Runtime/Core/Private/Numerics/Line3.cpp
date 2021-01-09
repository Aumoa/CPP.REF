// Copyright 2020 Aumoa.lib. All right reserved.

#include "Numerics/Line3.h"

#include "Mathematics/Math.h"
#include "HashHelper.h"
#include "CoreString.h"
#include "Numerics/Ray3.h"

using namespace std;

Line3::Line3()
{

}

Line3::Line3(const Vector3& start, const Vector3& end)
{
	Start = start;
	End = end;
}

Line3::Line3(const Line3& copy) : This(copy.Start, copy.End)
{

}

bool Line3::Equals(const Line3& rh) const
{
	return *this == rh;
}

bool Line3::NearlyEquals(const Line3& rh, float epsilon) const
{
	return Start.NearlyEquals(rh.Start, epsilon)
		&& Start.NearlyEquals(rh.End, epsilon);
}

size_t Line3::GetHashCode() const
{
	return Start.GetHashCode() ^ End.GetHashCode();
}

TRefPtr<String> Line3::ToString() const
{
	return String::Format(L"Start={0}, End={1}", Start, End);
}

Vector3 Line3::GetPoint(float factor) const
{
	return Start + Vector * factor;
}

Vector3 Line3::Vector_get() const
{
	return End - Start;
}

Line3 Line3::FromRay(const Ray3& ray)
{
	return Line3(ray.Origin, ray.GetEndPoint());
}