// Copyright 2020 Aumoa.lib. All right reserved.

#include "Numerics/Sphere.h"

#include "Mathematics/Math.h"
#include "HashHelper.h"
#include "CoreString.h"

using namespace std;

Sphere::Sphere()
{

}

Sphere::Sphere(const Vector3& center, float radius)
{
	Center = center;
	Radius = radius;
}

Sphere::Sphere(float cx, float cy, float cz, float radius) : This({ cx, cy, cz }, radius)
{

}

Sphere::Sphere(const Sphere& copy) : This(copy.Center, copy.Radius)
{

}

bool Sphere::Equals(const Sphere& rh) const
{
	return *this == rh;
}

bool Sphere::NearlyEquals(const Sphere& rh, float epsilon) const
{
	return Center.NearlyEquals(rh.Center, epsilon)
		&& Math::Abs(Radius - rh.Radius) < epsilon;
}

size_t Sphere::GetHashCode() const
{
	return Center.GetHashCode() ^ HashHelper::GetHashCode(Radius);
}

TRefPtr<String> Sphere::ToString() const
{
	return String::Format(L"Center={0}, Radius={1}", Center, Radius);
}