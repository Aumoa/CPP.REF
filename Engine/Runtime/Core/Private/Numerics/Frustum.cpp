// Copyright 2020 Aumoa.lib. All right reserved.

#include "Numerics/Frustum.h"

#include <cstring>
#include "Numerics/Matrix4x4.h"

Frustum::Frustum()
{

}

Frustum::Frustum(const Plane& left, const Plane& right, const Plane& top, const Plane& bottom, const Plane& near, const Plane& far)
{
	Left = left;
	Top = top;
	Right = right;
	Bottom = bottom;
	Near = near;
	Far = far;
}

Frustum::Frustum(const Plane* planes)
{
	memcpy(Planes, planes, sizeof(Plane) * 6);
}

Frustum Frustum::Construct(const Matrix4x4& inViewProj)
{
	Frustum f;

	// Left plane.
	f.Planes[0][0] = inViewProj._14 + inViewProj._11;
	f.Planes[0][1] = inViewProj._24 + inViewProj._21;
	f.Planes[0][2] = inViewProj._34 + inViewProj._31;
	f.Planes[0][3] = inViewProj._44 + inViewProj._41;
	f.Planes[0] = f.Planes[0].Normalized;

	// Top plane.
	f.Planes[1][0] = inViewProj._14 - inViewProj._12;
	f.Planes[1][1] = inViewProj._24 - inViewProj._22;
	f.Planes[1][2] = inViewProj._34 - inViewProj._32;
	f.Planes[1][3] = inViewProj._44 - inViewProj._42;
	f.Planes[1] = f.Planes[1].Normalized;

	// Right plane.
	f.Planes[2][0] = inViewProj._14 - inViewProj._11;
	f.Planes[2][1] = inViewProj._24 - inViewProj._21;
	f.Planes[2][2] = inViewProj._34 - inViewProj._31;
	f.Planes[2][3] = inViewProj._44 - inViewProj._41;
	f.Planes[2] = f.Planes[2].Normalized;

	// Bottom plane.
	f.Planes[3][0] = inViewProj._14 + inViewProj._12;
	f.Planes[3][1] = inViewProj._24 + inViewProj._22;
	f.Planes[3][2] = inViewProj._34 + inViewProj._32;
	f.Planes[3][3] = inViewProj._44 + inViewProj._42;
	f.Planes[3] = f.Planes[3].Normalized;

	// Near plane.
	f.Planes[4][0] = inViewProj._14 + inViewProj._13;
	f.Planes[4][1] = inViewProj._24 + inViewProj._23;
	f.Planes[4][2] = inViewProj._34 + inViewProj._33;
	f.Planes[4][3] = inViewProj._44 + inViewProj._43;
	f.Planes[4] = f.Planes[4].Normalized;

	// Far plane.
	f.Planes[5][0] = inViewProj._14 - inViewProj._13;
	f.Planes[5][1] = inViewProj._24 - inViewProj._23;
	f.Planes[5][2] = inViewProj._34 - inViewProj._33;
	f.Planes[5][3] = inViewProj._44 - inViewProj._43;
	f.Planes[5] = f.Planes[5].Normalized;

	return f;
}