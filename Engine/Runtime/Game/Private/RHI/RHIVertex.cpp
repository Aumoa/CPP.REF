// Copyright 2020 Aumoa.lib. All right reserved.

#include "RHI/RHIVertex.h"

RHIVertex::RHIVertex()
{

}

RHIVertex::RHIVertex(const Vector3& pos, const ::Color& color)
{
	Pos = pos;
	Color = color;
}

RHIVertex::~RHIVertex()
{

}