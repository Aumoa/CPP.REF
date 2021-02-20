// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Numerics/Line2.h"

#include "Mathematics/Math.h"
#include "HashHelper.h"
#include "CoreString.h"
#include "Numerics/Ray2.h"

using namespace std;

Line2::Line2()
{

}

Line2::Line2(const Vector2& start, const Vector2& end)
{
	Start = start;
	End = end;
}

Line2::Line2(const Line2& copy) : This(copy.Start, copy.End)
{

}

bool Line2::Equals(const Line2& rh) const
{
	return *this == rh;
}

bool Line2::NearlyEquals(const Line2& rh, float epsilon) const
{
	return Start.NearlyEquals(rh.Start, epsilon)
		&& Start.NearlyEquals(rh.End, epsilon);
}

size_t Line2::GetHashCode() const
{
	return Start.GetHashCode() ^ End.GetHashCode();
}

TRefPtr<String> Line2::ToString() const
{
	return String::Format(L"Start={0}, End={1}", Start, End);
}

Vector2 Line2::GetPoint(float factor) const
{
	return Start + Vector * factor;
}

Vector2 Line2::Vector_get() const
{
	return End - Start;
}

Line2 Line2::FromRay(const Ray2& ray)
{
	return Line2(ray.Origin, ray.GetEndPoint());
}