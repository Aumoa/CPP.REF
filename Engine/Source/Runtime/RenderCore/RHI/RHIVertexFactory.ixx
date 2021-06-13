// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.RenderCore:RHIVertexFactory;

import std.core;
import SC.Runtime.Core;
import :RHIDeviceChild;
import :RHIStructures;

using namespace std;

export class RHIVertexFactory : public RHIDeviceChild
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
	virtual vector<RHIVertexElement> GetVertexDeclaration() const { return {}; }
};