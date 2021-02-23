// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "RHIVertex.h"

class Material;

struct RHIStaticMeshSubsetInfo
{
	uint32 VertexStart;
	uint32 VertexCount;
	uint32 IndexStart;
	uint32 IndexCount;
};

struct RHIStaticMeshGeometryData
{
	std::span<const RHIVertex> VertexBuffer;
	std::span<const uint32> IndexBuffer;
	std::span<const RHIStaticMeshSubsetInfo> Subsets;
	std::span<Material* const> Materials;
};