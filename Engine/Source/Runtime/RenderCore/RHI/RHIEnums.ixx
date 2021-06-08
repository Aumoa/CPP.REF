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


/// <summary>
/// Declare pixel formats.
/// </summary>
export enum class ERHIPixelFormat
{
    Unknown	                                = 0,
    R32G32B32A32_TYPELESS                   = 1,
    R32G32B32A32_FLOAT                      = 2,
    R32G32B32A32_UINT                       = 3,
    R32G32B32A32_SINT                       = 4,
    R32G32B32_TYPELESS                      = 5,
    R32G32B32_FLOAT                         = 6,
    R32G32B32_UINT                          = 7,
    R32G32B32_SINT                          = 8,
    R16G16B16A16_TYPELESS                   = 9,
    R16G16B16A16_FLOAT                      = 10,
    R16G16B16A16_UNORM                      = 11,
    R16G16B16A16_UINT                       = 12,
    R16G16B16A16_SNORM                      = 13,
    R16G16B16A16_SINT                       = 14,
    R32G32_TYPELESS                         = 15,
    R32G32_FLOAT                            = 16,
    R32G32_UINT                             = 17,
    R32G32_SINT                             = 18,
    R32G8X24_TYPELESS                       = 19,
    D32_FLOAT_S8X24_UINT                    = 20,
    R32_FLOAT_X8X24_TYPELESS                = 21,
    X32_TYPELESS_G8X24_UINT                 = 22,
    R10G10B10A2_TYPELESS                    = 23,
    R10G10B10A2_UNORM                       = 24,
    R10G10B10A2_UINT                        = 25,
    R11G11B10_FLOAT                         = 26,
    R8G8B8A8_TYPELESS                       = 27,
    R8G8B8A8_UNORM                          = 28,
    R8G8B8A8_UNORM_SRGB                     = 29,
    R8G8B8A8_UINT                           = 30,
    R8G8B8A8_SNORM                          = 31,
    R8G8B8A8_SINT                           = 32,
    R16G16_TYPELESS                         = 33,
    R16G16_FLOAT                            = 34,
    R16G16_UNORM                            = 35,
    R16G16_UINT                             = 36,
    R16G16_SNORM                            = 37,
    R16G16_SINT                             = 38,
    R32_TYPELESS                            = 39,
    D32_FLOAT                               = 40,
    R32_FLOAT                               = 41,
    R32_UINT                                = 42,
    R32_SINT                                = 43,
    R24G8_TYPELESS                          = 44,
    D24_UNORM_S8_UINT                       = 45,
    R24_UNORM_X8_TYPELESS                   = 46,
    X24_TYPELESS_G8_UINT                    = 47,
    R8G8_TYPELESS                           = 48,
    R8G8_UNORM                              = 49,
    R8G8_UINT                               = 50,
    R8G8_SNORM                              = 51,
    R8G8_SINT                               = 52,
    R16_TYPELESS                            = 53,
    R16_FLOAT                               = 54,
    D16_UNORM                               = 55,
    R16_UNORM                               = 56,
    R16_UINT                                = 57,
    R16_SNORM                               = 58,
    R16_SINT                                = 59,
    R8_TYPELESS                             = 60,
    R8_UNORM                                = 61,
    R8_UINT                                 = 62,
    R8_SNORM                                = 63,
    R8_SINT                                 = 64,
    A8_UNORM                                = 65,
    R1_UNORM                                = 66,
    R9G9B9E5_SHAREDEXP                      = 67,
    R8G8_B8G8_UNORM                         = 68,
    G8R8_G8B8_UNORM                         = 69,
    BC1_TYPELESS                            = 70,
    BC1_UNORM                               = 71,
    BC1_UNORM_SRGB                          = 72,
    BC2_TYPELESS                            = 73,
    BC2_UNORM                               = 74,
    BC2_UNORM_SRGB                          = 75,
    BC3_TYPELESS                            = 76,
    BC3_UNORM                               = 77,
    BC3_UNORM_SRGB                          = 78,
    BC4_TYPELESS                            = 79,
    BC4_UNORM                               = 80,
    BC4_SNORM                               = 81,
    BC5_TYPELESS                            = 82,
    BC5_UNORM                               = 83,
    BC5_SNORM                               = 84,
    B5G6R5_UNORM                            = 85,
    B5G5R5A1_UNORM                          = 86,
    B8G8R8A8_UNORM                          = 87,
    B8G8R8X8_UNORM                          = 88,
    R10G10B10_XR_BIAS_A2_UNORM              = 89,
    B8G8R8A8_TYPELESS                       = 90,
    B8G8R8A8_UNORM_SRGB                     = 91,
    B8G8R8X8_TYPELESS                       = 92,
    B8G8R8X8_UNORM_SRGB                     = 93,
    BC6H_TYPELESS                           = 94,
    BC6H_UF16                               = 95,
    BC6H_SF16                               = 96,
    BC7_TYPELESS                            = 97,
    BC7_UNORM                               = 98,
    BC7_UNORM_SRGB                          = 99,
    AYUV                                    = 100,
    Y410                                    = 101,
    Y416                                    = 102,
    NV12                                    = 103,
    P010                                    = 104,
    P016                                    = 105,
    YUY2                                    = 107,
    Y210                                    = 108,
    Y216                                    = 109,
    NV11                                    = 110,
    AI44                                    = 111,
    IA44                                    = 112,
    P8                                      = 113,
    A8P8                                    = 114,
    B4G4R4A4_UNORM                          = 115,

    P208                                    = 130,
    V208                                    = 131,
    V408                                    = 132,


    SAMPLER_FEEDBACK_MIN_MIP_OPAQUE         = 189,
    SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE = 190,
};

/// <summary>
/// The vertex element format.
/// </summary>
export enum class ERHIVertexElementFormat
{
    Unknown	                                = 0,
    R32G32B32A32_FLOAT                      = 2,
    R32G32B32A32_UINT                       = 3,
    R32G32B32A32_SINT                       = 4,
    R32G32B32_FLOAT                         = 6,
    R32G32B32_UINT                          = 7,
    R32G32B32_SINT                          = 8,
    R16G16B16A16_FLOAT                      = 10,
    R16G16B16A16_UNORM                      = 11,
    R16G16B16A16_UINT                       = 12,
    R16G16B16A16_SNORM                      = 13,
    R16G16B16A16_SINT                       = 14,
    R32G32_FLOAT                            = 16,
    R32G32_UINT                             = 17,
    R32G32_SINT                             = 18,
    R8G8B8A8_UNORM                          = 28,
    R8G8B8A8_UINT                           = 30,
    R8G8B8A8_SNORM                          = 31,
    R8G8B8A8_SINT                           = 32,
    R16G16_FLOAT                            = 34,
    R16G16_UNORM                            = 35,
    R16G16_UINT                             = 36,
    R16G16_SNORM                            = 37,
    R16G16_SINT                             = 38,
    R8G8_UNORM                              = 49,
    R8G8_UINT                               = 50,
    R8G8_SNORM                              = 51,
    R8G8_SINT                               = 52,
    R16_FLOAT                               = 54,
    R16_UNORM                               = 56,
    R16_UINT                                = 57,
    R16_SNORM                               = 58,
    R16_SINT                                = 59,
    R8_UNORM                                = 61,
    R8_UINT                                 = 62,
    R8_SNORM                                = 63,
    R8_SINT                                 = 64,
};

/// <summary>
/// Input slot classification.
/// </summary>
export enum class ERHIInputClassification
{
    PerVertexData = 0,
    PerInstanceData = 1,
};