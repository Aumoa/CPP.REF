// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHIDeviceChild.h"
#include <vector>
#include "RHIStructures.h"

class RHIResource;

class RHIVertexFactory : public RHIDeviceChild
{
public:
	using Super = RHIDeviceChild;

public:
	RHIVertexFactory(RHIDevice* device);

	/// <summary>
	/// Create vertex buffer using vertex declaration of this shader program.
	/// </summary>
	virtual RHIResource* CreateVertexBuffer(const RHIVertex* vertices, size_t count) const;

	/// <summary>
	/// Create index buffer.
	/// </summary>
	virtual RHIResource* CreateIndexBuffer(const uint32* indices, size_t count) const;

	/// <summary>
	/// Get vertex stride.
	/// </summary>
	virtual uint32 GetVertexStride() const { return sizeof(RHIVertex); }

	/// <summary>
	/// Provide vertex declaration of this shader program.
	/// </summary>
	virtual std::vector<RHIVertexElement> GetVertexDeclaration() const { return {}; }
};