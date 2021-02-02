// Copyright 2020 Aumoa.lib. All right reserved.

#include "RHI/RHIVertex.h"

RHIVertex::RHIVertex()
{

}

RHIVertex::RHIVertex(const Vector3& pos, const Vector3& normal, const Vector2& texCoord, const ::Color& color)
{
	Pos = pos;
	Color = color;
	TexCoord = texCoord;
	Normal = normal;
}

RHIVertex::~RHIVertex()
{

}