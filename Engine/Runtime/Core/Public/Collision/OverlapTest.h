// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"
#include "Object.h"

struct Frustum;
struct Vector3;
struct Sphere;
struct AxisAlignedCube;

class CORE_API OverlapTest abstract : virtual public Object
{
public:
	using Super = Object;

public:
	static bool IsOverlap(const Frustum& inFrustum, const Vector3& inVector);
	static bool IsOverlap(const Vector3& inVector, const Frustum& inFrustum) { return IsOverlap(inFrustum, inVector); }
	static bool IsOverlap(const Frustum& inFrustum, const Sphere& inSphere);
	static bool IsOverlap(const Sphere& inSphere, const Frustum& inFrustum) { return IsOverlap(inFrustum, inSphere); }
	static bool IsOverlap(const Frustum& inFrustum, const AxisAlignedCube& inAABB);
	static bool IsOverlap(const AxisAlignedCube& inAABB, const Frustum& inFrustum) { return IsOverlap(inFrustum, inAABB); }
};