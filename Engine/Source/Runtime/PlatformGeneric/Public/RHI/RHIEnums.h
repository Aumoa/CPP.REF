// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Reflection/Enum.h"

SENUM(ERHIPrimitiveTopology, int32,
    TriangleList, 4,
    TriangleStrip, 5
);

SENUM(ERHICommandListType, int32,
    Direct, 0,
    Bundle, 1,
    Compute, 2,
    Copy, 3,
    VideoDecode, 4,
    VideoProcess, 5,
    VideoEncode, 6
);

SENUM(ERHIResourceStates, int32,
    Common, 0,
    VertexAndConstantBuffer, 0x1,
    IndexBuffer, 0x2,
    RenderTarget, 0x4,
    UnorderedAccess, 0x8,
    DepthWrite, 0x10,
    DepthRead, 0x20,
    NonPixelShaderResource, 0x40,
    PixelShaderResource, 0x80,
    StreamOut, 0x100,
    IndirectArgument, 0x200,
    CopyDest, 0x400,
    CopySource, 0x800,
    ResolveDest, 0x1000,
    ResolveSource, 0x2000,
    RaytracingAccelerationStructure, 0x400000,

    GenericRead, VertexAndConstantBuffer | IndexBuffer | NonPixelShaderResource | PixelShaderResource | IndirectArgument | CopySource,
    Present, 0
);

SENUM(ERHIPixelFormat, int32,
    Unknown, 0,
    R32G32B32A32_TYPELESS, 1,
    R32G32B32A32_FLOAT, 2,
    R32G32B32A32_UINT, 3,
    R32G32B32A32_SINT, 4,
    R32G32B32_TYPELESS, 5,
    R32G32B32_FLOAT, 6,
    R32G32B32_UINT, 7,
    R32G32B32_SINT, 8,
    R16G16B16A16_TYPELESS, 9,
    R16G16B16A16_FLOAT, 10,
    R16G16B16A16_UNORM, 11,
    R16G16B16A16_UINT, 12,
    R16G16B16A16_SNORM, 13,
    R16G16B16A16_SINT, 14,
    R32G32_TYPELESS, 15,
    R32G32_FLOAT, 16,
    R32G32_UINT, 17,
    R32G32_SINT, 18,
    R32G8X24_TYPELESS, 19,
    D32_FLOAT_S8X24_UINT, 20,
    R32_FLOAT_X8X24_TYPELESS, 21,
    X32_TYPELESS_G8X24_UINT, 22,
    R10G10B10A2_TYPELESS, 23,
    R10G10B10A2_UNORM, 24,
    R10G10B10A2_UINT, 25,
    R11G11B10_FLOAT, 26,
    R8G8B8A8_TYPELESS, 27,
    R8G8B8A8_UNORM, 28,
    R8G8B8A8_UNORM_SRGB, 29,
    R8G8B8A8_UINT, 30,
    R8G8B8A8_SNORM, 31,
    R8G8B8A8_SINT, 32,
    R16G16_TYPELESS, 33,
    R16G16_FLOAT, 34,
    R16G16_UNORM, 35,
    R16G16_UINT, 36,
    R16G16_SNORM, 37,
    R16G16_SINT, 38,
    R32_TYPELESS, 39,
    D32_FLOAT, 40,
    R32_FLOAT, 41,
    R32_UINT, 42,
    R32_SINT, 43,
    R24G8_TYPELESS, 44,
    D24_UNORM_S8_UINT, 45,
    R24_UNORM_X8_TYPELESS, 46,
    X24_TYPELESS_G8_UINT, 47,
    R8G8_TYPELESS, 48,
    R8G8_UNORM, 49,
    R8G8_UINT, 50,
    R8G8_SNORM, 51,
    R8G8_SINT, 52,
    R16_TYPELESS, 53,
    R16_FLOAT, 54,
    D16_UNORM, 55,
    R16_UNORM, 56,
    R16_UINT, 57,
    R16_SNORM, 58,
    R16_SINT, 59,
    R8_TYPELESS, 60,
    R8_UNORM, 61,
    R8_UINT, 62,
    R8_SNORM, 63,
    R8_SINT, 64,
    A8_UNORM, 65,
    R1_UNORM, 66,
    R9G9B9E5_SHAREDEXP, 67,
    R8G8_B8G8_UNORM, 68,
    G8R8_G8B8_UNORM, 69,
    BC1_TYPELESS, 70,
    BC1_UNORM, 71,
    BC1_UNORM_SRGB, 72,
    BC2_TYPELESS, 73,
    BC2_UNORM, 74,
    BC2_UNORM_SRGB, 75,
    BC3_TYPELESS, 76,
    BC3_UNORM, 77,
    BC3_UNORM_SRGB, 78,
    BC4_TYPELESS, 79,
    BC4_UNORM, 80,
    BC4_SNORM, 81,
    BC5_TYPELESS, 82,
    BC5_UNORM, 83,
    BC5_SNORM, 84,
    B5G6R5_UNORM, 85,
    B5G5R5A1_UNORM, 86,
    B8G8R8A8_UNORM, 87,
    B8G8R8X8_UNORM, 88,
    R10G10B10_XR_BIAS_A2_UNORM, 89,
    B8G8R8A8_TYPELESS, 90,
    B8G8R8A8_UNORM_SRGB, 91,
    B8G8R8X8_TYPELESS, 92,
    B8G8R8X8_UNORM_SRGB, 93,
    BC6H_TYPELESS, 94,
    BC6H_UF16, 95,
    BC6H_SF16, 96,
    BC7_TYPELESS, 97,
    BC7_UNORM, 98,
    BC7_UNORM_SRGB, 99,
    AYUV, 100,
    Y410, 101,
    Y416, 102,
    NV12, 103,
    P010, 104,
    P016, 105,
    YUY2, 107,
    Y210, 108,
    Y216, 109,
    NV11, 110,
    AI44, 111,
    IA44, 112,
    P8, 113,
    A8P8, 114,
    B4G4R4A4_UNORM, 115,

    P208, 130,
    V208, 131,
    V408, 132,


    SAMPLER_FEEDBACK_MIN_MIP_OPAQUE, 189,
    SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE, 190
);

SENUM(ERHIVertexElementFormat, int32,
    Unknown, 0,
    R32G32B32A32_FLOAT, 2,
    R32G32B32A32_UINT, 3,
    R32G32B32A32_SINT, 4,
    R32G32B32_FLOAT, 6,
    R32G32B32_UINT, 7,
    R32G32B32_SINT, 8,
    R16G16B16A16_FLOAT, 10,
    R16G16B16A16_UNORM, 11,
    R16G16B16A16_UINT, 12,
    R16G16B16A16_SNORM, 13,
    R16G16B16A16_SINT, 14,
    R32G32_FLOAT, 16,
    R32G32_UINT, 17,
    R32G32_SINT, 18,
    R8G8B8A8_UNORM, 28,
    R8G8B8A8_UINT, 30,
    R8G8B8A8_SNORM, 31,
    R8G8B8A8_SINT, 32,
    R16G16_FLOAT, 34,
    R16G16_UNORM, 35,
    R16G16_UINT, 36,
    R16G16_SNORM, 37,
    R16G16_SINT, 38,
    R32_FLOAT, 41,
    R32_UINT, 42,
    R32_SINT, 43,
    R8G8_UNORM, 49,
    R8G8_UINT, 50,
    R8G8_SNORM, 51,
    R8G8_SINT, 52,
    R16_FLOAT, 54,
    R16_UNORM, 56,
    R16_UINT, 57,
    R16_SNORM, 58,
    R16_SINT, 59,
    R8_UNORM, 61,
    R8_UINT, 62,
    R8_SNORM, 63,
    R8_SINT, 64
);

SENUM(ERHIInputClassification, int32,
    PerVertexData, 0,
    PerInstanceData, 1
);

SENUM(ERHIShaderParameterType, int32,
    ParameterCollection_CameraConstants,,

    ParameterCollection,,
    ScalarParameterConstants,,
    StructuredBuffer,,
    DescriptorTable,
);

SENUM(ERHIResourceFlags, int32,
    None, 0,
    AllowRenderTarget, 0x1,
    AllowDepthStencil, 0x2,
    AllowUnorderedAccess, 0x4,
    DenyShaderResource, 0x8,
    AllowCrossAdapter, 0x10,
    AllowSimultaneous, 0x20,
    VideoDecodeReferenceOnly, 0x40
);

SENUM(ERHIDescriptorRangeType, int32,
    ShaderResourceView,,
    UnorderedAccessView,,
    ConstantBufferView,,
    Sampler,
);

SENUM(ERHIBufferUsage, int32,
    Default,,
    Immutable,,
    Dynamic,
);

SENUM(ERHIRTVDimension, int32,
    Unknown,,
    Buffer,,
    Texture1D,,
    Texture1DArray,,
    Texture2D,,
    Texture2DArray,,
    Texture2DMS,,
    Texture2DMSArray,,
    Texture3D,
);

SENUM(ERHIDSVDimension, int32,
    Unknown,,
    Texture1D,,
    Texture1DArray,,
    Texture2D,,
    Texture2DArray,,
    Texture2DMS,,
    Texture2DMSArray,
);

SENUM(ERHISRVDimension, int32,
    Unknown,,
    Buffer,,
    Texture1D,,
    Texture1DArray,,
    Texture2D,,
    Texture2DArray,,
    Texture2DMS,,
    Texture2DMSArray,,
    Texture3D,,
    Texture3DCube,,
    Texture3DCubeArray,,
    RaytracingAccelerationStructure,
);

SENUM(ERHIDSVFlags, int32,
    None,,
    ReadOnlyDepth, 0x01,
    ReadOnlyStencil, 0x02
);

SENUM(ERHIBufferSRVFlags, int32,
    None,,
    Raw, 0x1
);

SENUM(ERHIFontWeight, int32,
    Thin, 100,
    ExtraLight, 200,
    UltraLight, 200,
    Light, 300,
    SemiLight, 350,
    Normal, 400,
    Regular, 400,
    Medium, 500,
    DemiBold, 600,
    SemiBold, 600,
    Bold, 700,
    ExtraBold, 800,
    UltraBold, 800,
    Black, 900,
    Heavy, 900,
    ExtraBlack, 950,
    UltraBlack, 950
);

SENUM(ERHIFontStyle, int32,
    Normal,,
    Oblique,,
    Italic,
);

SENUM(ERHIFontStretch, int32,
    Undefined, 0,
    UltraCondensed, 1,
    ExtraCondensed, 2,
    Condensed, 3,
    SemiCondensed, 4,
    Normal, 5,
    Medium, 5,
    SemiExpanded, 6,
    Expanded, 7,
    ExtraExpanded, 8,
    UltraExpanded, 9
);

SENUM(EMaterialBlendMode, int32,
    Opaque,,
    Masked,,
    Transparent,,
    UI,
);

SENUM(EMaterialDomain, int32,
    Surface,,
    UserInterface,
);

SENUM(ERHIDrawTextOptions, int32,
    None, 0,
    NoSnap, 0x1,
    Clip, 0x2,
    EnableColorFont, 0x4,
    DisableColorBitmapSnapping, 0x8
);

SENUM(ERHITextAlignment, int32,
    Leading,,
    Trailing,,
    Center,,
    Justified,
);

SENUM(ERHIParagraphAlignment, int32,
    Near,,
    Far,,
    Center,
);

SENUM(ERHIInterpolationMode, int32,
    NearestNeighbor,,
    Linear,,
    Cubic,,
    MultiSampleLinear,,
    Anisotropic,,
    HighQualityCubic,
);

SENUM(ERHICompositeMode, int32,
    SourceOver,,
    DestinationOver,,
    SourceIn,,
    DestinationIn,,
    SourceOut,,
    DestinationOut,,
    SourceAtop,,
    DestinationAtop,,
    Xor,,
    Plus,,
    SourceCopy,,
    BoundedSourceCopy,,
    MaskInvert,
);

SENUM(ERHIAttachmentLoadOp, int32,
    None, 0,
    Load, 1,
    Clear, 2
);

SENUM(ERHIAttachmentStoreOp, int32,
    None,,
    Store,
);

SENUM(ERHIShaderType, int32,
    VertexShader,,
    PixelShader,,
    DomainShader,,
    HullShader,,
    GeometryShader,,
    ComputeShader,
);

SENUM(ERHIRootParameterType, int32,
    DescriptorTable, 0,
    _32BitConstants,,
    ConstantBuffer,,
    ShaderResource,,
    UnorderedAccess,
);

SENUM(ERHIShaderVisibility, int32,
    All, 0,
    Vertex,,
    Hull,,
    Domain,,
    Geometry,,
    Pixel,,
    Amplification,,
    Mesh,
);

SENUM(ERHISamplerFilter, int32,
    MIN_MAG_MIP_POINT, 0,
    MIN_MAG_POINT_MIP_LINEAR, 0x1,
    MIN_POINT_MAG_LINEAR_MIP_POINT, 0x4,
    MIN_POINT_MAG_MIP_LINEAR, 0x5,
    MIN_LINEAR_MAG_MIP_POINT, 0x10,
    MIN_LINEAR_MAG_POINT_MIP_LINEAR, 0x11,
    MIN_MAG_LINEAR_MIP_POINT, 0x14,
    MIN_MAG_MIP_LINEAR, 0x15,
    ANISOTROPIC, 0x55,
    COMPARISON_MIN_MAG_MIP_POINT, 0x80,
    COMPARISON_MIN_MAG_POINT_MIP_LINEAR, 0x81,
    COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT, 0x84,
    COMPARISON_MIN_POINT_MAG_MIP_LINEAR, 0x85,
    COMPARISON_MIN_LINEAR_MAG_MIP_POINT, 0x90,
    COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR, 0x91,
    COMPARISON_MIN_MAG_LINEAR_MIP_POINT, 0x94,
    COMPARISON_MIN_MAG_MIP_LINEAR, 0x95,
    COMPARISON_ANISOTROPIC, 0xd5,
    MINIMUM_MIN_MAG_MIP_POINT, 0x100,
    MINIMUM_MIN_MAG_POINT_MIP_LINEAR, 0x101,
    MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT, 0x104,
    MINIMUM_MIN_POINT_MAG_MIP_LINEAR, 0x105,
    MINIMUM_MIN_LINEAR_MAG_MIP_POINT, 0x110,
    MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR, 0x111,
    MINIMUM_MIN_MAG_LINEAR_MIP_POINT, 0x114,
    MINIMUM_MIN_MAG_MIP_LINEAR, 0x115,
    MINIMUM_ANISOTROPIC, 0x155,
    MAXIMUM_MIN_MAG_MIP_POINT, 0x180,
    MAXIMUM_MIN_MAG_POINT_MIP_LINEAR, 0x181,
    MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT, 0x184,
    MAXIMUM_MIN_POINT_MAG_MIP_LINEAR, 0x185,
    MAXIMUM_MIN_LINEAR_MAG_MIP_POINT, 0x190,
    MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR, 0x191,
    MAXIMUM_MIN_MAG_LINEAR_MIP_POINT, 0x194,
    MAXIMUM_MIN_MAG_MIP_LINEAR, 0x195,
    MAXIMUM_ANISOTROPIC, 0x1d5
);

SENUM(ERHITextureAddressMode, int32,
    Wrap, 1,
    Mirror, 2,
    Clamp, 3,
    Border, 4,
    MirrorOnce, 5
);

SENUM(ERHIComparisonFunc, int32,
    Never, 1,
    Less, 2,
    Equal, 3,
    LessEqual, 4,
    Greater, 5,
    NotEqual, 6,
    GreaterEqual, 7,
    Always, 8
);

SENUM(ERHIStaticBorderColor, int32,
    TransparentBlack, 0,
    OpaqueBlack, TransparentBlack + 1,
    OpaqueWhite, OpaqueBlack + 1
);

SENUM(ERHIRootSignatureFlags, int32,
    None, 0,
    AllowInputAssemblerInputLayout, 0x1,
    DenyVertexShaderRootAccess, 0x2,
    DenyHullShaderRootAccess, 0x4,
    DenyDomainShaderRootAccess, 0x8,
    DenyGeometryShaderRootAccess, 0x10,
    DenyPixelShaderRootAccess, 0x20,
    AllowStreamOutput, 0x40,
    LocalRootSignature, 0x80,
    DenyAmplificationShaderRootAccess, 0x100,
    DenyMeshShaderRootAccess, 0x200,
    CbvSrvUavHeapDirectlyIndexed, 0x400,
    SamplerHeapDirectlyIndexed, 0x800
);

SENUM(ERHIBlend, int32,
    Zero, 1,
    One, 2,
    SrcColor, 3,
    InvSrcColor, 4,
    SrcAlpha, 5,
    InvSrcAlpha, 6,
    DestAlpha, 7,
    InvDestAlpha, 8,
    DestColor, 9,
    InvDestColor, 10,
    SrcAlphaSat, 11,
    BlendFactor, 14,
    InvBlendFactor, 15,
    Src1Color, 16,
    InvSrc1Color, 17,
    Src1Alpha, 18,
    InvSrc1Alpha, 19
);

SENUM(ERHIBlendOp, int32,
    Add, 1,
    Subtract, 2,
    RevSubtract, 3,
    Min, 4,
    Max, 5
);

SENUM(ERHILogicOp, int32,
    Clear, 0,
    Set, (Clear + 1),
    Copy, (Set + 1),
    CopyInverted, (Copy + 1),
    Noop, (CopyInverted + 1),
    Invert, (Noop + 1),
    And, (Invert + 1),
    Nand, (And + 1),
    Or, (Nand + 1),
    Nor, (Or + 1),
    Xor, (Nor + 1),
    Equiv, (Xor + 1),
    AndReverse, (Equiv + 1),
    AndInverted, (AndReverse + 1),
    OrReverse, (AndInverted + 1),
    OrInverted, (OrReverse + 1)
);

SENUM(ERHIFillMode, int32,
    Wireframe, 2,
    Solid, 3
);

SENUM(ERHICullMode, int32,
    None, 1,
    Front, 2,
    Back, 3
);

SENUM(ERHIConservativeRasterizationMode, int32,
    Off, 0,
    On, 1
);

SENUM(ERHIDepthWriteMask, int32,
    Zero, 0,
    All, 1
);

SENUM(ERHIStencilOp, int32,
    Keep, 1,
    Zero, 2,
    Replace, 3,
    Incr_sat, 4,
    Decr_sat, 5,
    Invert, 6,
    Incr, 7,
    Decr, 8
);

SENUM(ERHIIndexBufferStripCutValue, int32,
    Disabled, 0,
    FFFF, 1,
    FFFFFFFF, 2
);

SENUM(ERHIPrimitiveTopologyType, int32,
    Undefined, 0,
    Point, 1,
    Line, 2,
    Triangle, 3,
    Patch, 4
);

SENUM(ERHIPipelineStateFlags, int32,
    None, 0,
    ToolDebug, 1
);

SENUM(ERHIColorWriteEnable, uint8,
    Red, 1,
    Green, 2,
    Blue, 4,
    Alpha, 8,
    All, (Red | Green | Blue | Alpha)
);

SENUM(ERHIResourceBarrierType, int32,
    Transition, 0,
    Aliasing, Transition + 1,
    UAV, Aliasing + 1
);

SENUM(ERHIResourceBarrierFlags, int32,
    None, 0,
    BeginOnly, 0x1,
    EndOnly, 0x2
);

SENUM(ERHIUAVDimension, int32,
    Unknown, 0,
    Buffer, 1,
    Texture1D, 2,
    Texture1DArray, 3,
    Texture2D, 4,
    Texture2DArray, 5,
    Texture3D, 8
);

SENUM(ERHIBufferUAVFlags, int32,
    None, 0,
    Raw, 0x1
);

SENUM(ERHIHeapType, int32,
    Default, 1,
    Upload, 2,
    Readback, 3,
    Custom, 4
);

SENUM(ERHICPUPageProperty, int32,
    Unknown, 0,
    NotAvailable, 1,
    WriteCombine, 2,
    WriteBack, 3
);

SENUM(ERHIMemoryPool, int32,
    Unknown, 0,
    L0, 1,
    L1, 2
);

SENUM(ERHIHeapFlags, int32,
    None, 0,
    Shared, 0x1,
    DenyBuffers, 0x4,
    AllowDisplay, 0x8,
    SharedCrossAdapter, 0x20,
    DenyRtDsTextures, 0x40,
    DenyNonRtDsTextures, 0x80,
    HardwareProtected, 0x100,
    AllowWriteWatch, 0x200,
    AllowShaderAtomics, 0x400,
    CreateNotResident, 0x800,
    CreateNotZeroed, 0x1000,
    AllowAllBuffersAndTextures, 0,
    AllowOnlyBuffers, 0xc0,
    AllowOnlyNonRtDsTextures, 0x44,
    AllowOnlyRtDsTextures, 0x84
);

SENUM(ERHIResourceDimension, int32,
    Unknown, 0,
    Buffer, 1,
    Texture1D, 2,
    Texture2D, 3,
    Texture3D, 4
);

SENUM(ERHITextureLayout, int32,
    Unknown, 0,
    RowMajor, 1,
    UndefinedSwizzle64KB, 2,
    StandardSwizzle64KB, 3
);