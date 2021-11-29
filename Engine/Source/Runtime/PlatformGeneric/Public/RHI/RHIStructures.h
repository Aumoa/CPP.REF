// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHIEnums.h"
#include <variant>

interface IRHIResource;

#pragma pack(push, 4)

/// <summary>
/// Describes the dimensions of a viewport.
/// </summary>
struct RHIViewport
{
	/// <summary>
	/// X position of the left hand side of the viewport.
	/// </summary>
	float TopLeftX;

	/// <summary>
	/// Y position of the top of the viewport.
	/// </summary>
	float TopLeftY;

	/// <summary>
	/// Width of the viewport.
	/// </summary>
	float Width;

	/// <summary>
	/// Height of the viewport.
	/// </summary>
	float Height;

	/// <summary>
	/// Minimum depth of the viewport. Ranges between 0 and 1.
	/// </summary>
	float MinDepth;

	/// <summary>
	/// Maximum depth of the viewport. Ranges between 0 and 1.
	/// </summary>
	float MaxDepth;

	/// <summary>
	/// New <see cref="RHIViewport"/> instance.
	/// </summary>
	RHIViewport() = default;

	/// <summary>
	/// Initialize new <see cref="RHIViewport"/> instance.
	/// </summary>
	constexpr RHIViewport(float topLeftX, float topLeftY, float width, float height, float minDepth, float maxDepth)
		: TopLeftX(topLeftX)
		, TopLeftY(topLeftY)
		, Width(width)
		, Height(height)
		, MinDepth(minDepth)
		, MaxDepth(maxDepth)
	{
	}

	/// <summary>
	/// Get total depth space by 1 dimension value.
	/// </summary>
	constexpr float GetTotalSpaceAlongZ() const
	{
		return MaxDepth - MinDepth;
	}
};

/// <summary>
/// Describes the scissor area of viewport.
/// </summary>
struct RHIScissorRect
{
	/// <summary>
	/// Left position of the scissor area.
	/// </summary>
	int32 Left;

	/// <summary>
	/// Top position of the scissor area.
	/// </summary>
	int32 Top;

	/// <summary>
	/// Right position of the scissor area.
	/// </summary>
	int32 Right;

	/// <summary>
	/// Bottom position of the scissor area.
	/// </summary>
	int32 Bottom;

	/// <summary>
	/// New <see cref="RHIScissorRect"/> instance.
	/// </summary>
	RHIScissorRect() = default;

	/// <summary>
	/// Initialize new <see cref="RHIScissorRect"/> instance.
	/// </summary>
	constexpr RHIScissorRect(int32 left, int32 top, int32 right, int32 bottom)
		: Left(left)
		, Top(top)
		, Right(right)
		, Bottom(bottom)
	{
	}

	/// <summary>
	/// Get width.
	/// </summary>
	constexpr int32 GetTotalSpaceAlongVertical() const
	{
		return Right - Left;
	}

	/// <summary>
	/// Get height.
	/// </summary>
	constexpr int32 GetTotalSpaceAlongHorizontal() const
	{
		return Bottom - Top;
	}
};

/// <summary>
/// Describes the transition of subresources between different usages.
/// </summary>
struct RHIResourceTransitionBarrier
{
	/// <summary>
	/// A pointer to the IRHIResource object that represents the resource used in the transition.
	/// </summary>
	IRHIResource* pResource;

	/// <summary>
	/// The index of the subresource for the transition. Use the std::numeric_limits<uint32>::max() to transition all subresources in a resource at the same time.
	/// </summary>
	uint32 Subresource;

	/// <summary>
	/// The "before" usages of the subresources, as a bitwise-OR'd combination of ERHIResourceStates enumeration constants.
	/// </summary>
	ERHIResourceStates StateBefore;

	/// <summary>
	/// The "after" usages of the subresources, as a bitwise-OR'd combination of ERHIResourceStates enumeration constants.
	/// </summary>
	ERHIResourceStates StateAfter;

	RHIResourceTransitionBarrier& SwapTransition() &
	{
		std::swap(StateBefore, StateAfter);
		return *this;
	}

	RHIResourceTransitionBarrier&& SwapTransition() &&
	{
		std::swap(StateBefore, StateAfter);
		return std::move(*this);
	}

	RHIResourceTransitionBarrier GetSwapTransition() const
	{
		return RHIResourceTransitionBarrier(*this).SwapTransition();
	}
};

/// <summary>
/// Describes the transition between usages of two different resources that have mappings into the same heap.
/// </summary>
struct RHIResourceAliasingBarrier
{
	/// <summary>
	/// A pointer to the IRHIResource object that represents the before resource used in the transition.
	/// </summary>
	IRHIResource* pResourceBefore;

	/// <summary>
	/// A pointer to the IRHIResource object that represents the after resource used in the transition.
	/// </summary>
	IRHIResource* pResourceAfter;
};

/// <summary>
/// Represents a resource in which all UAV accesses must complete before any future UAV accesses can begin.
/// </summary>
struct RHIResourceUAVBarrier
{
	/// <summary>
	/// The resource used in the transition, as a pointer to IRHIResource.
	/// </summary>
	IRHIResource* pResource;
};

/// <summary>
/// An variant type of each resource barriers.
/// </summary>
using RHIResourceBarrier = std::variant<RHIResourceTransitionBarrier, RHIResourceAliasingBarrier, RHIResourceUAVBarrier>;

/// <summary>
/// Represents vertex element using input layout.
/// </summary>
struct RHIVertexElement
{
	/// <summary>
	/// The semantic name.
	/// </summary>
	std::string SemanticName;

	/// <summary>
	/// The semantic index.
	/// </summary>
	uint32 SemanticIndex = 0;

	/// <summary>
	/// Buffer stride.
	/// </summary>
	uint32 AlignedByteOffset = 0;

	/// <summary>
	/// The vertex element format.
	/// </summary>
	ERHIVertexElementFormat Format = ERHIVertexElementFormat::Unknown;

	/// <summary>
	/// The input slot.
	/// </summary>
	uint32 InputSlot = 0;

	/// <summary>
	/// The input slot classification.
	/// </summary>
	ERHIInputClassification InputSlotClass = ERHIInputClassification::PerVertexData;
};

/// <summary>
/// The full-spec vertex declaration.
/// </summary>
struct RHIVertex
{
	/// <summary>
	/// The position.
	/// </summary>
	Vector3 Position;

	/// <summary>
	/// The normal vector.
	/// </summary>
	Vector3 Normal;

	/// <summary>
	/// Color.
	/// </summary>
	Color Color = NamedColors::White;

	/// <summary>
	/// Texture coordinates.
	/// </summary>
	Vector2 TexCoord;
};

/// <summary>
/// Represents vertex buffer view.
/// </summary>
struct RHIVertexBufferView
{
	uint64 BufferLocation = 0;
	uint32 SizeInBytes = 0;
	uint32 StrideInBytes = 0;
};

struct RHIIndexBufferView
{
	uint64 BufferLocation = 0;
	uint32 SizeInBytes = 0;
	ERHIVertexElementFormat Format = ERHIVertexElementFormat::R32_UINT;
};

/// <summary>
/// Describe parameter collection shader parameter.
/// </summary>
struct RHIParameterCollectionDeclaration
{
	uint32 ShaderRegister;
	uint32 RegisterSpace;
};

struct RHIScalarConstantsParameter
{
	uint32 ShaderRegister;
	uint32 RegisterSpace;
	uint32 Num32Bits;
};

struct RHIStructuredBufferParameter
{
	uint32 ShaderRegister;
};

struct RHIDescriptorRange
{
	ERHIDescriptorRangeType RangeType;
	uint32 NumDescriptors;
	uint32 BaseShaderRegister;
	uint32 RegisterSpace;
};

struct RHIDescriptorTableParameter
{
	uint32 NumDescriptorRanges;
	const RHIDescriptorRange* pDescriptorRanges;
};

/// <summary>
/// Represents shader parameter element.
/// </summary>
struct RHIShaderParameterElement
{
	ERHIShaderParameterType Type = ERHIShaderParameterType::ParameterCollection;
	union
	{
		RHIParameterCollectionDeclaration ParameterCollection;
		RHIScalarConstantsParameter ScalarConstantsParameter;
		RHIStructuredBufferParameter StructuredBuffer;
		RHIDescriptorTableParameter DescriptorTable;
	};
};

struct alignas(256) RHIViewConstants
{
	Matrix4x4 World;
	Matrix4x4 WorldViewProj;
	Matrix4x4 WorldInvTranspose;
};

struct RHITexture2DClearValue
{
	ERHIPixelFormat Format;
	union
	{
		float ClearColor[4];
		struct DepthStencilValue
		{
			float Depth;
			uint8 Stencil;
		} DepthStencil;
	};
};

struct RHISampleDesc
{
	uint32 Count;
	uint32 Quality;
};

struct RHITexture2DDesc
{
	uint32 Width;
	uint32 Height;
	uint16 DepthOrArraySize;
	uint16 MipLevels;
	ERHIPixelFormat Format;
	ERHIBufferUsage Usage;
	ERHIResourceFlags Flags;
	RHISampleDesc SampleDesc;
	ERHIResourceStates InitialState;
	std::optional<RHITexture2DClearValue> ClearValue;
};

struct RHISubresourceData
{
	const void* pSysMem;
	size_t SysMemPitch;
	size_t SysMemSlicePitch;
};

struct RHIBufferDesc
{
	uint32 ByteWidth;
	ERHIBufferUsage Usage;
	ERHIResourceStates InitialState;
	uint32 StructureByteStride;
	ERHIResourceFlags Flags;
};

struct RHIBufferRTV
{
	uint64 FirstElement;
	uint32 NumElements;
};

struct RHITex1DRTV
{
	uint32 MipSlice;
};

struct RHITex1DArrayRTV
{
	uint32 MipSlice;
	uint32 FirstArraySlice;
	uint32 ArraySize;
};

struct RHITex2DRTV
{
	uint32 MipSlice;
	uint32 PlaneSlice;
};

struct RHITex2DMSRTV
{
};

struct RHITex2DArrayRTV
{
	uint32 MipSlice;
	uint32 FirstArraySlice;
	uint32 ArraySize;
	uint32 PlaneSlice;
};

struct RHITex2DMSArrayRTV
{
	uint32 FirstArraySlice;
	uint32 ArraySize;
};

struct RHITex3DRTV
{
	uint32 MipSlice;
	uint32 FirstWSlice;
	uint32 WSize;
};

struct RHITex1DDSV
{
	uint32 MipSlice;
};

struct RHITex1DArrayDSV
{
	uint32 MipSlice;
	uint32 FirstArraySlice;
	uint32 ArraySize;
};

struct RHITex2DDSV
{
	uint32 MipSlice;
};

struct RHITex2DArrayDSV
{
	uint32 MipSlice;
	uint32 FirstArraySlice;
	uint32 ArraySize;
};

struct RHITex2DMSDSV
{
};

struct RHITex2DMSArrayDSV
{
	uint32 FirstArraySlice;
	uint32 ArraySize;
};

struct RHIBufferSRV
{
	uint64 FirstElement;
	uint32 NumElements;
	uint32 StructureByteStride;
	ERHIBufferSRVFlags Flags;
};

struct RHITex1DSRV
{
	uint32 MostDetailedMip;
	uint32 MipLevels;
	float ResourceMinLODClamp;
};

struct RHITex1DArraySRV
{
	uint32 MostDetailedMip;
	uint32 MipLevels;
	uint32 FirstArraySlice;
	uint32 ArraySize;
	float ResourceMinLODClamp;
};

struct RHITex2DSRV
{
	uint32 MostDetailedMip;
	uint32 MipLevels;
	uint32 PlaneSlice;
	float ResourceMinLODClamp;
};

struct RHITex2DArraySRV
{
	uint32 MostDetailedMip;
	uint32 MipLevels;
	uint32 FirstArraySlice;
	uint32 ArraySize;
	uint32 PlaneSlice;
	float ResourceMinLODClamp;
};

struct RHITex3DSRV
{
	uint32 MostDetailedMip;
	uint32 MipLevels;
	float ResourceMinLODClamp;
};

struct RHITexCubeSRV
{
	uint32 MostDetailedMip;
	uint32 MipLevels;
	float ResourceMinLODClamp;
};

struct RHITexCubeArraySRV
{
	uint32 MostDetailedMip;
	uint32 MipLevels;
	uint32 First2DArrayFace;
	uint32 NumCubes;
	float ResourceMinLODClamp;
};

struct RHITex2DMSSRV
{
};

struct RHITex2DMSArraySRV
{
	uint32 FirstArraySlice;
	uint32 ArraySize;
};

struct RHIRaytracingAccelerationStructureSRV
{
	uint64 Location;
};

struct RHIRenderTargetViewDesc
{
	ERHIPixelFormat Format;
	ERHIRTVDimension ViewDimension;
	union
	{
		RHIBufferRTV Buffer;
		RHITex1DRTV Texture1D;
		RHITex1DArrayRTV Texture1DArray;
		RHITex2DRTV Texture2D;
		RHITex2DArrayRTV Texture2DArray;
		RHITex2DMSRTV Texture2DMS;
		RHITex2DMSArrayRTV Texture2DMSArray;
		RHITex3DRTV Texture3D;
	};
};

struct RHIDepthStencilViewDesc
{
	ERHIPixelFormat Format;
	ERHIDSVDimension ViewDimension;
	ERHIDSVFlags Flags;
	union
	{
		RHITex1DDSV Texture1D;
		RHITex1DArrayDSV Texture1DArray;
		RHITex2DDSV Texture2D;
		RHITex2DArrayDSV Texture2DArray;
		RHITex2DMSDSV Texture2DMS;
		RHITex2DMSArrayDSV Texture2DMSArray;
	};
};

struct RHIShaderResourceViewDesc
{
	ERHIPixelFormat Format;
	ERHISRVDimension ViewDimension;
	uint32 Shader4ComponentMapping;
	union
	{
		RHIBufferSRV Buffer;
		RHITex1DSRV Texture1D;
		RHITex1DArraySRV Texture1DArray;
		RHITex2DSRV Texture2D;
		RHITex2DArraySRV Texture2DArray;
		RHITex2DMSSRV Texture2DMS;
		RHITex2DMSArraySRV Texture2DMSArray;
		RHITex3DSRV Texture3D;
		RHITexCubeSRV TextureCube;
		RHITexCubeArraySRV TextureCubeArray;
		RHIRaytracingAccelerationStructureSRV RaytracingAccelerationStructure;
	};

	static constexpr uint32 EncodeShader4ComponentMapping(uint8 Src0, uint8 Src1, uint8 Src2, uint8 Src3)
	{
		constexpr uint32 Mask = 0x7;
		constexpr uint32 Shift = 3;
		constexpr uint32 AlwaysSetBitAvoidingZeroMemMistakes = 1 << (Shift * 4);

		return 
			(((Src0)&Mask) |
			(((Src1)&Mask) << Shift) |
			(((Src2)&Mask) << (Shift * 2)) |
			(((Src3)&Mask) << (Shift * 3)) |
			AlwaysSetBitAvoidingZeroMemMistakes);
	}

	static constexpr uint8 DecodeShader4ComponentMapping(uint32 ComponentToExtract, uint32 Mapping)
	{
		constexpr uint32 Mask = 0x7;
		constexpr uint32 Shift = 3;
		return (uint8)(Mapping >> (Shift * ComponentToExtract) & Mask);
	}

	static constexpr uint32 DefaultShader4ComponentMapping()
	{
		return EncodeShader4ComponentMapping(0, 1, 2, 3);
	}
};

struct RHIMaterialParameterInfo
{
	std::wstring Name;
	ERHIShaderParameterType ElementType;
	int32 Index;
};

#pragma pack(pop)