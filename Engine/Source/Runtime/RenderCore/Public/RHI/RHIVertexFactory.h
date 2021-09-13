// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHIDeviceChild.h"
#include <vector>
#include "RHIStructures.h"

class SRHIResource;

class RENDERCORE_API SRHIVertexFactory : public SRHIDeviceChild
{
	GENERATED_BODY(SRHIVertexFactory)

public:
	SRHIVertexFactory(SRHIDevice* device);

	/// <summary>
	/// Create vertex buffer using vertex declaration of this shader program.
	/// </summary>
	virtual SRHIResource* CreateVertexBuffer(const RHIVertex* vertices, size_t count) const;

	/// <summary>
	/// Create index buffer.
	/// </summary>
	virtual SRHIResource* CreateIndexBuffer(const uint32* indices, size_t count) const;

	/// <summary>
	/// Get vertex stride.
	/// </summary>
	virtual uint32 GetVertexStride() const { return sizeof(RHIVertex); }

	/// <summary>
	/// Provide vertex declaration of this shader program.
	/// </summary>
	virtual std::vector<RHIVertexElement> GetVertexDeclaration() const { return {}; }

	/// <summary>
	/// Get index stride.
	/// </summary>
	virtual uint32 GetIndexStride() const { return sizeof(uint32); }
};