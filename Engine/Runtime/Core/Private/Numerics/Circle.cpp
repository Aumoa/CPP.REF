// Copyright 2020 Aumoa.lib. All right reserved.

#include "Numerics/Circle.h"

#include "Mathematics/Math.h"
#include "Mathematics/Math.inl"
#include "HashHelper.h"
#include "CoreString.h"

using namespace std;

Circle::Circle()
{

}

Circle::Circle(const Vector2& center, float radius)
{
	Center = center;
	Radius = radius;
}

Circle::Circle(float cx, float cy, float radius) : This({ cx, cy }, radius)
{

}

Circle::Circle(const Circle& copy) : This(copy.Center, copy.Radius)
{

}

bool Circle::Equals(const Circle& rh) const
{
	return *this == rh;
}

bool Circle::NearlyEquals(const Circle& rh, float epsilon) const
{
	return Center.NearlyEquals(rh.Center, epsilon)
		&& Math::Abs(Radius - rh.Radius) < epsilon;
}

size_t Circle::GetHashCode() const
{
	return Center.GetHashCode() ^ HashHelper::GetHashCode(Radius);
}

TRefPtr<String> Circle::ToString() const
{
	return String::Format(L"Center={0}, Radius={1}", Center, Radius);
}