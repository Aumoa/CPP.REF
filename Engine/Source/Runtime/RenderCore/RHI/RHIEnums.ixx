// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.RenderCore:RHIEnums;

import SC.Runtime.Core;

#define DEFINE_ENUM_FLAG_OPERATORS(Enum) \
export\
{\
	inline constexpr Enum operator |(Enum a, Enum b) { return Enum(((int32)a) | ((int32)b)); }\
	inline Enum& operator |=(Enum& a, Enum b) { return (Enum&)(((int32 &)a) |= ((int32)b)); }\
	inline constexpr Enum operator &(Enum a, Enum b) { return Enum(((int32)a) & ((int32)b)); }\
	inline Enum& operator &=(Enum& a, Enum b) { return (Enum&)(((int32 &)a) &= ((int32)b)); }\
	inline constexpr Enum operator ~(Enum a) { return Enum(~((int32)a)); }\
	inline constexpr Enum operator ^(Enum a, Enum b) { return Enum(((int32)a) ^ ((int32)b)); }\
	inline Enum& operator ^=(Enum& a, Enum b) { return (Enum&)(((int32 &)a) ^= ((int32)b)); }\
}

/// <summary>
/// Represents primitive topology.
/// </summary>
export enum class ERHIPrimitiveTopology
{
	/// <summary>
	/// The triangle list topology.
	/// </summary>
	TriangleList = 4,

	/// <summary>
	/// The triangle strip topology.
	/// </summary>
	TriangleStrip = 5
};

/// <summary>
/// Specifies the type of a command list that written by device context.
/// </summary>
export enum class ERHICommandType
{
	/// <summary>
	/// Specifies a command buffer that the GPU can execute. A direct device context doesn't inherit any GPU state.
	/// </summary>
	Direct = 0,

	/// <summary>
	/// Specifies a command buffer that can be executed only directly via a direct device context. A bundle device context inherits all GPU state (except for the currently set pipeline state object and primitive topology).
	/// </summary>
	Bundle = 1,

	/// <summary>
	/// Specifies a command buffer for computing.
	/// </summary>
	Compute = 2,

	/// <summary>
	/// Specifies a command buffer for copying.
	/// </summary>
	Copy = 3,
};

/// <summary>
/// Specifies the resource state for guarding resource usage.
/// </summary>
export enum class ERHIResourceStates
{
	Common = 0,
	VertexAndConstantBuffer = 0x1,
	IndexBuffer = 0x2,
	RenderTarget = 0x4,
	UnorderedAccess = 0x8,
	DepthWrite = 0x10,
	DepthRead = 0x20,
	NonPixelShaderResource = 0x40,
	PixelShaderResource = 0x80,
	StreamOut = 0x100,
	IndirectArgument = 0x200,
	CopyDest = 0x400,
	CopySource = 0x800,
	ResolveDest = 0x1000,
	ResolveSource = 0x2000,
	RaytracingAccelerationStructure = 0x400000,

	GENERIC_READ = VertexAndConstantBuffer | IndexBuffer | NonPixelShaderResource | PixelShaderResource | IndirectArgument | CopySource,
	Present = 0,
};

DEFINE_ENUM_FLAG_OPERATORS(ERHIResourceStates);