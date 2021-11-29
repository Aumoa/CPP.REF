// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "MeshBatchElement.h"
//#include "VertexFactory.h"
#include "RHI/RHIStructures.h"

class SMaterialInterface;
class SVertexFactory;

struct MeshBatch
{
	uint64 VertexBufferLocation;
	uint64 IndexBufferLocation;
	std::vector<RHIVertex> VertexBuffer;
	std::vector<uint32> IndexBuffer;

	std::vector<MeshBatchElement> Elements;
	std::vector<SMaterialInterface*> MaterialSlots;
	//SVertexFactory* VertexFactory;

	//RHIVertexBufferView GetVertexBufferView() const
	//{
	//	uint32 Stride = VertexFactory->GetVertexStride();

	//	return RHIVertexBufferView
	//	{
	//		.BufferLocation = VertexBufferLocation,
	//		.SizeInBytes = Stride * (uint32)VertexBuffer.size(),
	//		.StrideInBytes = Stride
	//	};
	//}

	//RHIIndexBufferView GetIndexBufferView() const
	//{
	//	uint32 Stride = VertexFactory->GetIndexStride();

	//	return RHIIndexBufferView
	//	{
	//		.BufferLocation = IndexBufferLocation,
	//		.SizeInBytes = Stride * (uint32)IndexBuffer.size(),
	//		.Format = VertexFactory->GetIndexFormat(),
	//	};
	//}
};