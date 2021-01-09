// Copyright 2020 Aumoa.lib. All right reserved.

#include "Physics/Collision.h"

#include <limits>
#include "Numerics/Rect.h"
#include "Numerics/Vector2.h"
#include "Numerics/Ray2.h"
#include "Numerics/AxisAlignedCube.h"
#include "Numerics/Ray3.h"
#include "Mathematics/Math.h"

using namespace std;

bool Collision::IsOverlap(const Rect& rc, const Vector2& pt)
{
	return Math::InRange(pt, rc.Min, rc.Max);
}

bool Collision::IsOverlap(const Rect& rc, const Ray2& ray)
{
	SweepResult2D _;
	return QuerySweepRay2Rect(ray, rc, _);
}

bool Collision::IsOverlap(const Rect& rc, const Rect& rc2)
{
	return !(rc.Min.X > rc2.Max.X ||
			 rc.Max.X < rc2.Min.X ||
			 rc.Min.Y > rc2.Max.Y ||
			 rc.Max.Y < rc2.Min.Y);
}

bool Collision::IsOverlap(const AxisAlignedCube& cube, const AxisAlignedCube& cube2)
{
	return !(
		cube.Min.X > cube2.Max.X ||
		cube.Max.X < cube2.Min.X ||
		cube.Min.Y > cube2.Max.Y ||
		cube.Max.Y < cube2.Min.Y ||
		cube.Min.Z > cube2.Max.Z ||
		cube.Max.Z < cube2.Min.Z);
}

bool Collision::IsOverlap(const AxisAlignedCube& cube, const Vector3& pt)
{
	return Math::InRange(pt, cube.Min, cube.Max);
}

bool Collision::IsOverlap(const AxisAlignedCube& cube, const Ray3& ray)
{
	SweepResult3D _;
	return QuerySweepRay3AABB(ray, cube, _);
}

bool Collision::QuerySweepRay2Rect(const Ray2& ray, const Rect& rc, SweepResult2D& outSweepResult)
{
	outSweepResult.bBlockingHit = false;
	outSweepResult.TraceStart = ray.Origin;
	outSweepResult.TraceEnd = ray.GetEndPoint();

	if (IsOverlap(rc, ray.Origin))
	{
		outSweepResult.bBlockingHit = true;
		outSweepResult.ImpactPoint = outSweepResult.TraceStart;
		outSweepResult.ImpactNormal = (ray.Origin - rc.Center).Normalized;
		outSweepResult.HitTime = 0;
		outSweepResult.Impulse = ray.Distance.has_value() ? ray.Distance.value() : numeric_limits<float>::infinity();
		return true;
	}

	Vector2 dirinv = 1.0f / ray.Direction;

	float t1 = (rc.Min.X - ray.Origin.X) * dirinv.X;
	float t2 = (rc.Max.X - ray.Origin.X) * dirinv.X;
	float t3 = (rc.Min.Y - ray.Origin.Y) * dirinv.Y;
	float t4 = (rc.Max.Y - ray.Origin.Y) * dirinv.Y;

	float tmin = Math::Max(Math::Min(t1, t2), Math::Min(t3, t4));
	float tmax = Math::Min(Math::Max(t1, t2), Math::Max(t3, t4));

	if (tmax < 0)
	{
		return false;
	}

	if (tmin > tmax)
	{
		return false;
	}

	if (tmin < 0)
	{
		return false;
	}

	if (ray.Distance.has_value() && ray.Distance.value() > tmin)
	{
		return false;
	}

	if (tmin == t1)
	{
		outSweepResult.ImpactNormal = Vector2(-1, 0);
	}
	else if (tmin == t2)
	{
		outSweepResult.ImpactNormal = Vector2(1, 0);
	}
	else if (tmin == t3)
	{
		outSweepResult.ImpactNormal = Vector2(0, -1);
	}
	else if (tmin == t4)
	{
		outSweepResult.ImpactNormal = Vector2(0, 1);
	}

	outSweepResult.bBlockingHit = true;
	outSweepResult.ImpactPoint = ray.GetPoint(tmin);
	outSweepResult.HitTime = tmin;
	outSweepResult.Impulse = ray.Distance.has_value() ? (ray.Distance.value() - tmin) : numeric_limits<float>::infinity();

	return true;
}

bool Collision::QuerySweepRay3AABB(const Ray3& ray, const AxisAlignedCube& aabb, SweepResult3D& outSweepResult)
{
	outSweepResult.bBlockingHit = false;
	outSweepResult.TraceStart = ray.Origin;
	outSweepResult.TraceEnd = ray.GetEndPoint();

	if (IsOverlap(aabb, ray.Origin))
	{
		outSweepResult.bBlockingHit = true;
		outSweepResult.ImpactPoint = outSweepResult.TraceStart;
		outSweepResult.ImpactNormal = (ray.Origin - aabb.Center).Normalized;
		outSweepResult.HitTime = 0;
		outSweepResult.Impulse = ray.Distance.has_value() ? ray.Distance.value() : numeric_limits<float>::infinity();
		return true;
	}

	Vector3 dirinv = 1.0f / ray.Direction;

	float t1 = (aabb.Min.X - ray.Origin.X) * dirinv.X;
	float t2 = (aabb.Max.X - ray.Origin.X) * dirinv.X;
	float t3 = (aabb.Min.Y - ray.Origin.Y) * dirinv.Y;
	float t4 = (aabb.Max.Y - ray.Origin.Y) * dirinv.Y;
	float t5 = (aabb.Min.Z - ray.Origin.Z) * dirinv.Z;
	float t6 = (aabb.Max.Z - ray.Origin.Z) * dirinv.Z;

	float tmin = Math::Max(Math::Min(t1, t2), Math::Min(t3, t4), Math::Min(t5, t6));
	float tmax = Math::Min(Math::Max(t1, t2), Math::Max(t3, t4), Math::Max(t5, t6));

	if (tmax < 0)
	{
		return false;
	}

	if (tmin > tmax)
	{
		return false;
	}

	if (tmin < 0)
	{
		return false;
	}

	if (ray.Distance.has_value() && ray.Distance.value() > tmin)
	{
		return false;
	}

	if (tmin == t1)
	{
		outSweepResult.ImpactNormal = Vector3(-1, 0, 0);
	}
	else if (tmin == t2)
	{
		outSweepResult.ImpactNormal = Vector3(1, 0, 0);
	}
	else if (tmin == t3)
	{
		outSweepResult.ImpactNormal = Vector3(0, -1, 0);
	}
	else if (tmin == t4)
	{
		outSweepResult.ImpactNormal = Vector3(0, 1, 0);
	}
	else if (tmin == t5)
	{
		outSweepResult.ImpactNormal = Vector3(0, 0, -1);
	}
	else if (tmin == t6)
	{
		outSweepResult.ImpactNormal = Vector3(0, 0, 1);
	}

	outSweepResult.bBlockingHit = true;
	outSweepResult.ImpactPoint = ray.GetPoint(tmin);
	outSweepResult.HitTime = tmin;
	outSweepResult.Impulse = ray.Distance.has_value() ? (ray.Distance.value() - tmin) : numeric_limits<float>::infinity();

	return true;
}