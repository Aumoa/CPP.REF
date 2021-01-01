// Copyright 2020 Aumoa.lib. All right reserved.

#include "Numerics/AxisAlignedCube.h"

#include "CoreString.h"
#include "Numerics/Ray3.h"

using namespace std;

AxisAlignedCube::AxisAlignedCube()
{

}

AxisAlignedCube::AxisAlignedCube(float x, float y, float z, float width, float height, float depth) : This(Vector3(x, y, z), Vector3(width * 0.5f, height * 0.5f, depth * 0.5f))
{

}

AxisAlignedCube::AxisAlignedCube(const Vector3& center, const Vector3& extent)
	: Center(center)
	, Extent(extent)
{
	
}

AxisAlignedCube::AxisAlignedCube(const AxisAlignedCube& copy)
	: Center(copy.Center)
	, Extent(copy.Extent)
{

}

bool AxisAlignedCube::Equals(const AxisAlignedCube& rh) const
{
	return Center == rh.Center
		&& Extent == rh.Extent;
}

bool AxisAlignedCube::NearlyEquals(const AxisAlignedCube& rh, float epsilon) const
{
	return Center.NearlyEquals(rh.Center, epsilon)
		&& Extent.NearlyEquals(rh.Extent, epsilon);
}

size_t AxisAlignedCube::GetHashCode() const
{
	return Center.GetHashCode() ^ Extent.GetHashCode();
}

TRefPtr<String> AxisAlignedCube::ToString() const
{
	return String::Format("{{Center: {0}, Extent: {1}}}", Center, Extent);
}

bool AxisAlignedCube::IsOverlap(const Vector3& point) const
{
	if (point.X >= Left && point.X <= Right &&
		point.Y >= Bottom && point.Y <= Top &&
		point.Z >= Near && point.Y <= Far)
	{
		return true;
	}

	return false;
}

bool AxisAlignedCube::IsOverlap(const AxisAlignedCube& cube) const
{
	return !(
		Left > cube.Right ||
		Right < cube.Left ||
		Bottom > cube.Top ||
		Top < cube.Bottom ||
		Near > cube.Far ||
		Far < cube.Near);
}

bool AxisAlignedCube::IsOverlap(const Ray3& ray) const
{
	return IsIntersect(ray).has_value();
}

optional<AxisAlignedCube> AxisAlignedCube::IsIntersect(const AxisAlignedCube& cube) const
{
	float left = max(Left, cube.Left);
	float bottom = max(Bottom, cube.Bottom);
	float near = max(Near, cube.Near);
	float right = min(Right, cube.Right);
	float top = min(Top, cube.Top);
	float far = min(Far, cube.Far);

	if (right >= left && top >= bottom && far >= near)
	{
		return AxisAlignedCube(left, top, right, bottom, near, far);
	}
	else
	{
		return nullopt;
	}
}

optional<float> AxisAlignedCube::IsIntersect(const Ray3& ray) const
{
	Vector3 dirinv = 1.0 / ray.Direction;

	float t1 = (Left - ray.Origin.X) * dirinv.X;
	float t2 = (Right - ray.Origin.X) * dirinv.X;
	float t3 = (Bottom - ray.Origin.Y) * dirinv.Y;
	float t4 = (Top - ray.Origin.Y) * dirinv.Y;
	float t5 = (Near - ray.Origin.Z) * dirinv.Z;
	float t6 = (Far - ray.Origin.Z) * dirinv.Z;

	float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
	float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

	if (tmax < 0)
	{
		return nullopt;
	}

	if (tmin > tmax)
	{
		return nullopt;
	}

	return tmin;
}

float AxisAlignedCube::Left_get() const
{
	return Center.X - Extent.X;
}

float AxisAlignedCube::Right_get() const
{
	return Center.X + Extent.X;
}

float AxisAlignedCube::Top_get() const
{
	return Center.Y + Extent.Y;
}

float AxisAlignedCube::Bottom_get() const
{
	return Center.Y - Extent.Y;
}

float AxisAlignedCube::Near_get() const
{
	return Center.Z - Extent.Z;
}

float AxisAlignedCube::Far_get() const
{
	return Center.Z + Extent.Z;
}

bool AxisAlignedCube::operator ==(const AxisAlignedCube& right) const
{
	return Center == right.Center && Extent == right.Extent;
}

bool AxisAlignedCube::operator !=(const AxisAlignedCube& right) const
{
	return Center != right.Center || Extent != right.Extent;
}

bool AxisAlignedCube::operator < (const AxisAlignedCube& right) const
{
	if (Center < right.Center)
	{
		return true;
	}
	else if (Center > right.Center)
	{
		return false;
	}
	else if (Extent < right.Extent)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool AxisAlignedCube::operator <=(const AxisAlignedCube& right) const
{
	if (Center < right.Center)
	{
		return true;
	}
	else if (Center > right.Center)
	{
		return false;
	}
	else if (Extent <= right.Extent)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool AxisAlignedCube::operator > (const AxisAlignedCube& right) const
{
	if (Center > right.Center)
	{
		return true;
	}
	else if (Center < right.Center)
	{
		return false;
	}
	else if (Extent > right.Extent)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool AxisAlignedCube::operator >=(const AxisAlignedCube& right) const
{
	if (Center > right.Center)
	{
		return true;
	}
	else if (Center < right.Center)
	{
		return false;
	}
	else if (Extent >= right.Extent)
	{
		return true;
	}
	else
	{
		return false;
	}
}

weak_ordering AxisAlignedCube::operator <=>(const AxisAlignedCube& right) const
{
	if (Center < right.Center)
	{
		return weak_ordering::less;
	}
	else if (Center > right.Center)
	{
		return weak_ordering::greater;
	}
	else if (Extent < right.Extent)
	{
		return weak_ordering::less;
	}
	else if (Extent > right.Extent)
	{
		return weak_ordering::greater;
	}
	else
	{
		return weak_ordering::equivalent;
	}
}