// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

struct GAME_API SweepResult2D
{
	bool bBlockingHit;
	Vector2 TraceStart;
	Vector2 TraceEnd;
	Vector2 ImpactPoint;
	Vector2 ImpactNormal;
	float HitTime;
	float Impulse;
};

struct GAME_API SweepResult3D
{
	bool bBlockingHit;
	Vector3 TraceStart;
	Vector3 TraceEnd;
	Vector3 ImpactPoint;
	Vector3 ImpactNormal;
	float HitTime;
	float Impulse;
};

class GAME_API Collision abstract : virtual public Object
{
public:
	static bool IsOverlap(const Rect & rc, const Vector2 & pt);
	static bool IsOverlap(const Rect& rc, const Ray2& ray);
	static bool IsOverlap(const Rect& rc, const Rect& rc2);
	static bool IsOverlap(const AxisAlignedCube& cube, const AxisAlignedCube& cube2);
	static bool IsOverlap(const AxisAlignedCube& cube, const Vector3& pt);
	static bool IsOverlap(const AxisAlignedCube& cube, const Ray3& ray);

	static bool QuerySweepRay2Rect(const Ray2& ray, const Rect& rc, SweepResult2D& outSweepResult);
	static bool QuerySweepRay3AABB(const Ray3& ray, const AxisAlignedCube& aabb, SweepResult3D& outSweepResult);
};