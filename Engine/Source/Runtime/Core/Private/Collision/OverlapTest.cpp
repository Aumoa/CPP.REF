// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Collision/OverlapTest.h"

#include "Numerics/Frustum.h"
#include "Numerics/Vector3.h"
#include "Numerics/Sphere.h"
#include "Numerics/AxisAlignedCube.h"

bool OverlapTest::IsOverlap(const Frustum& inFrustum, const Vector3& inVector)
{
	for (size_t i = 0; i < 6; i++)
	{
		if (Plane::DotCoord(inFrustum.Planes[i], inVector) < 0.0f)
		{
			return false;
		}
	}

	return true;
}

bool OverlapTest::IsOverlap(const Frustum& inFrustum, const Sphere& inSphere)
{
	for (size_t i = 0; i < 6; i++)
	{
		if (Plane::DotCoord(inFrustum.Planes[i], inSphere.Center) < -inSphere.Radius)
		{
			return false;
		}
	}

	return true;
}

bool OverlapTest::IsOverlap(const Frustum& inFrustum, const AxisAlignedCube& inAABB)
{
	for (size_t i = 0; i < 6; i++)
	{
		if (Plane::DotCoord(inFrustum.Planes[i], Vector3(inAABB.Min.X, inAABB.Min.Y, inAABB.Min.Z)) >= 0.0f)
		{
			continue;
		}

		if (Plane::DotCoord(inFrustum.Planes[i], Vector3(inAABB.Max.X, inAABB.Min.Y, inAABB.Min.Z)) >= 0.0f)
		{
			continue;
		}

		if (Plane::DotCoord(inFrustum.Planes[i], Vector3(inAABB.Min.X, inAABB.Max.Y, inAABB.Min.Z)) >= 0.0f)
		{
			continue;
		}

		if (Plane::DotCoord(inFrustum.Planes[i], Vector3(inAABB.Min.X, inAABB.Min.Y, inAABB.Max.Z)) >= 0.0f)
		{
			continue;
		}

		if (Plane::DotCoord(inFrustum.Planes[i], Vector3(inAABB.Max.X, inAABB.Max.Y, inAABB.Min.Z)) >= 0.0f)
		{
			continue;
		}

		if (Plane::DotCoord(inFrustum.Planes[i], Vector3(inAABB.Max.X, inAABB.Min.Y, inAABB.Max.Z)) >= 0.0f)
		{
			continue;
		}

		if (Plane::DotCoord(inFrustum.Planes[i], Vector3(inAABB.Min.X, inAABB.Max.Y, inAABB.Max.Z)) >= 0.0f)
		{
			continue;
		}

		if (Plane::DotCoord(inFrustum.Planes[i], Vector3(inAABB.Max.X, inAABB.Max.Y, inAABB.Max.Z)) >= 0.0f)
		{
			continue;
		}

		return false;
	}

	return true;
}