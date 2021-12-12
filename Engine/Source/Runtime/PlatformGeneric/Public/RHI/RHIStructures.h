// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHIEnums.h"
#include <variant>

interface IRHIResource;

#pragma pack(push, 4)

struct RHIViewport
{
	float TopLeftX;
	float TopLeftY;
	float Width;
	float Height;
	float MinDepth;
	float MaxDepth;

	RHIViewport() = default;
	constexpr RHIViewport(float topLeftX, float topLeftY, float width, float height, float minDepth, float maxDepth)
		: TopLeftX(topLeftX)
		, TopLeftY(topLeftY)
		, Width(width)
		, Height(height)
		, MinDepth(minDepth)
		, MaxDepth(maxDepth)
	{
	}

	constexpr float GetTotalSpaceAlongZ() const
	{
		return MaxDepth - MinDepth;
	}
};

struct RHIScissorRect
{
	int32 Left;
	int32 Top;
	int32 Right;
	int32 Bottom;

	RHIScissorRect() = default;
	constexpr RHIScissorRect(int32 left, int32 top, int32 right, int32 bottom)
		: Left(left)
		, Top(top)
		, Right(right)
		, Bottom(bottom)
	{
	}

	constexpr int32 GetTotalSpaceAlongVertical() const
	{
		return Right - Left;
	}

	constexpr int32 GetTotalSpaceAlongHorizontal() const
	{
		return Bottom - Top;
	}
};

struct RHIResourceTransitionBarrier
{
	std::shared_ptr<IRHIResource> pResource;
	uint32 Subresource;
	ERHIResourceStates StateBefore;
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

struct RHIResourceAliasingBarrier
{
	std::shared_ptr<IRHIResource> pResourceBefore;
	std::shared_ptr<IRHIResource> pResourceAfter;
};

struct RHIResourceUAVBarrier
{
	std::shared_ptr<IRHIResource> pResource;
};

using RHIResourceBarrier = std::variant<RHIResourceTransitionBarrier, RHIResourceAliasingBarrier, RHIResourceUAVBarrier>;

struct RHIVertexElement
{
	std::string SemanticName;
	uint32 SemanticIndex = 0;
	uint32 AlignedByteOffset = 0;
	ERHIVertexElementFormat Format = ERHIVertexElementFormat::Unknown;
	uint32 InputSlot = 0;
	ERHIInputClassification InputSlotClass = ERHIInputClassification::PerVertexData;
};

struct RHIVertex
{
	Vector3 Position;
	Vector3 Normal;
	Color Color = NamedColors::White;
	Vector2 TexCoord;
};

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