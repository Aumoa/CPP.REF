// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <vector>
#include "RHI/RHIStructures.h"

interface IRHIDevice;
interface IRHIBuffer;

class RENDERCORE_API SVertexFactory : implements SObject
{
	GENERATED_BODY(SVertexFactory)

private:
	IRHIDevice* _device = nullptr;

public:
	SVertexFactory(IRHIDevice* device);

	/// <summary>
	/// Create vertex buffer using vertex declaration of this shader program.
	/// </summary>
	virtual IRHIBuffer* CreateVertexBuffer(const RHIVertex* vertices, size_t count) const;

	/// <summary>
	/// Create index buffer.
	/// </summary>
	virtual IRHIBuffer* CreateIndexBuffer(const uint32* indices, size_t count) const;

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

	/// <summary>
	/// Get index format.
	/// </summary>
	virtual ERHIVertexElementFormat GetIndexFormat() const { return ERHIVertexElementFormat::R32_UINT; }
};