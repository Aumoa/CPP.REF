// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"
#include "TNumericsBasicOperator.h"

#include "Plane.h"

struct Matrix4x4;

struct CORE_API Frustum : public TNumericsBasicOperator<Frustum, Plane, Plane, Plane, Plane, Plane, Plane>
{
	Plane Planes[6];

	Frustum();
	Frustum(const Plane& left, const Plane& top, const Plane& right, const Plane& bottom, const Plane& near, const Plane& far);
	Frustum(const Plane* planes);

	vs_property_auto(Plane, Left, Planes[0]);
	vs_property_auto(Plane, Top, Planes[1]);
	vs_property_auto(Plane, Right, Planes[2]);
	vs_property_auto(Plane, Bottom, Planes[3]);
	vs_property_auto(Plane, Near, Planes[4]);
	vs_property_auto(Plane, Far, Planes[5]);

	static Frustum Construct(const Matrix4x4& inViewProj);
};