// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "RHI/RHIVertex.h"

RHIVertex::RHIVertex()
{

}

RHIVertex::RHIVertex(const Vector3& pos, const Vector3& normal, const Vector2& texCoord)
{
	Pos = pos;
	TexCoord = texCoord;
	Normal = normal;
}

RHIVertex::~RHIVertex()
{

}