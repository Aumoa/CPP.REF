// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHIEnums.h"
#include "Numerics/VectorInterface/Color.h"

class RHIResource;
class RHIRootSignature;
class RHIShaderBytecode;

using RHIGPUVirtualAddress = uint64;

struct RHIVertex
{
	Vector3 Position;
	Vector3 Normal;
	Color Color;
	Vector2 TexCoord;
};

struct RHIViewport
{
	float TopLeftX;
	float TopLeftY;
	float Width;
	float Height;
	float MinDepth;
	float MaxDepth;

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
	RHIResource* pResource;
	uint32 Subresource;
	ERHIResourceStates StateBefore;
	ERHIResourceStates StateAfter;
};

struct RHIResourceAliasingBarrier
{
	RHIResource* pResourceBefore;
	RHIResource* pResourceAfter;
};

struct RHIResourceUAVBarrier
{
	RHIResource* pResource;
};

struct RHIResourceBarrier
{
	ERHIResourceBarrierType Type;
	ERHIResourceBarrierFlags Flags;
	union
	{
		RHIResourceTransitionBarrier Transition;
		RHIResourceAliasingBarrier Aliasing;
		RHIResourceUAVBarrier UAV;
	};
};

struct RHIVertexBufferView
{
	RHIGPUVirtualAddress BufferLocation;
	uint32 SizeInBytes;
	uint32 StrideInBytes;
};

struct RHIIndexBufferView
{
	RHIGPUVirtualAddress BufferLocation;
	uint32 SizeInBytes;
	ERHIVertexElementFormat Format;
};

struct RHIDepthStencilValue
{
	float Depth;
	uint8 Stencil;
};

struct RHIClearValue
{
	ERHIPixelFormat Format;
	union
	{
		float Color[4];
		RHIDepthStencilValue DepthStencil;
	};
};

struct RHISampleDesc
{
	uint32 Count;
	uint32 Quality;
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

struct RHIBufferUAV
{
	uint64 FirstElement;
	uint32 NumElements;
	uint32 StructureByteStride;
	uint64 CounterOffsetInBytes;
	ERHIBufferUAVFlags Flags;
};

struct RHITex1DUAV
{
	uint32 MipSlice;
};

struct RHITex1DArrayUAV
{
	uint32 MipSlice;
	uint32 FirstArraySlice;
	uint32 ArraySize;
};

struct RHITex2DUAV
{
	uint32 MipSlice;
	uint32 PlaneSlice;
};

struct RHITex2DArrayUAV
{
	uint32 MipSlice;
	uint32 FirstArraySlice;
	uint32 ArraySize;
	uint32 PlaneSlice;
};

struct RHITex3DUAV
{
	uint32 MipSlice;
	uint32 FirstWSlice;
	uint32 WSize;
};

struct RHIUnorderedAccessViewDesc
{
	ERHIPixelFormat Format;
	ERHIUAVDimension ViewDimension;
	union
	{
		RHIBufferUAV Buffer;
		RHITex1DUAV Texture1D;
		RHITex1DArrayUAV Texture1DArray;
		RHITex2DUAV Texture2D;
		RHITex2DArrayUAV Texture2DArray;
		RHITex3DUAV Texture3D;
	};
};

struct RHIMaterialParameterInfo
{
	std::wstring Name;
	ERHIShaderParameterType ElementType;
	int32 Index;
};

struct RHIRenderPassAttachment
{
	ERHIPixelFormat Format;
	RHISampleDesc SampleDesc;
	ERHIAttachmentLoadOp LoadOp;
	ERHIAttachmentStoreOp StoreOp;
	ERHIResourceStates BeforeState;
	ERHIResourceStates AfterState;
};

struct RHIShaderCode
{
	std::string HLSLCode;
	std::string EntryPoint;
	ERHIShaderType ShaderType;
};

struct RHIDescriptorRange
{
	ERHIDescriptorRangeType RangeType;
	uint32 NumDescriptors;
	uint32 BaseShaderRegister;
	uint32 RegisterSpace;
	uint32 OffsetInDescriptorsFromTableStart;

	static constexpr uint32 DescriptorRangeOffsetAppend = 0xFFFFFFFF;
};

struct RHIRootDescriptorTable
{
	uint32 NumDescriptorRanges;
	const RHIDescriptorRange* pDescriptorRanges;
};

struct RHIRootConstants
{
	uint32 ShaderRegister;
	uint32 RegisterSpace;
	uint32 Num32BitValues;
};

struct RHIRootDescriptor
{
	uint32 ShaderRegister;
	uint32 RegisterSpace;
};

struct RHIRootParameter
{
	ERHIRootParameterType ParameterType;
	union
	{
		RHIRootDescriptorTable DescriptorTable;
		RHIRootConstants Constants;
		RHIRootDescriptor Descriptor;
	};
	ERHIShaderVisibility ShaderVisibility;
};

struct RHIStaticSamplerDesc
{
	ERHISamplerFilter Filter;
	ERHITextureAddressMode AddressU;
	ERHITextureAddressMode AddressV;
	ERHITextureAddressMode AddressW;
	float MipLODBias;
	uint32 MaxAnisotropy;
	ERHIComparisonFunc ComparisonFunc;
	ERHIStaticBorderColor BorderColor;
	float MinLOD;
	float MaxLOD;
	uint32 ShaderRegister;
	uint32 RegisterSpace;
	ERHIShaderVisibility ShaderVisibility;
};

struct RHIRootSignatureDesc
{
	uint32 NumParameters;
	const RHIRootParameter* pParameters;
	uint32 NumStaticSamplers;
	const RHIStaticSamplerDesc* pStaticSamplers;
	ERHIRootSignatureFlags Flags;
};

struct RHIStreamOutputDeclarationEntry
{
	uint32 Stream;
	const char* SemanticName;
	uint32 SemanticIndex;
	uint8 StartComponent;
	uint8 ComponentCount;
	uint8 OutputSlot;
};

struct RHIStreamOutputDesc
{
	const RHIStreamOutputDeclarationEntry* pSODeclaration;
	uint32 NumEntries;
	const uint32* pBufferStrides;
	uint32 NumStrides;
	uint32 RasterizedStream;
};

struct RHIRenderTargetBlendDesc
{
	bool BlendEnable;
	bool LogicOpEnable;
	ERHIBlend SrcBlend;
	ERHIBlend DestBlend;
	ERHIBlendOp BlendOp;
	ERHIBlend SrcBlendAlpha;
	ERHIBlend DestBlendAlpha;
	ERHIBlendOp BlendOpAlpha;
	ERHILogicOp LogicOp;
	ERHIColorWriteEnable RenderTargetWriteMask;
};

struct RHIBlendDesc
{
	bool AlphaToCoverageEnable;
	bool IndependentBlendEnable;
	RHIRenderTargetBlendDesc RenderTarget[8];
};

struct RHIRasterizerDesc
{
	ERHIFillMode FillMode;
	ERHICullMode CullMode;
	bool FrontCounterClockwise;
	int32 DepthBias;
	float DepthBiasClamp;
	float SlopeScaledDepthBias;
	bool DepthClipEnable;
	bool MultisampleEnable;
	bool AntialiasedLineEnable;
	uint32 ForcedSampleCount;
	ERHIConservativeRasterizationMode ConservativeRaster;
};

struct RHIDepthStencilOpDesc
{
	ERHIStencilOp StencilFailOp;
	ERHIStencilOp StencilDepthFailOp;
	ERHIStencilOp StencilPassOp;
	ERHIComparisonFunc StencilFunc;
};

struct RHIDepthStencilDesc
{
	bool DepthEnable;
	ERHIDepthWriteMask DepthWriteMask;
	ERHIComparisonFunc DepthFunc;
	bool StencilEnable;
	uint8 StencilReadMask;
	uint8 StencilWriteMask;
	RHIDepthStencilOpDesc FrontFace;
	RHIDepthStencilOpDesc BackFace;
};

struct RHIInputElementDesc
{
	const char* SemanticName;
	uint32 SemanticIndex;
	ERHIVertexElementFormat Format;
	uint32 InputSlot;
	uint32 AlignedByteOffset;
	ERHIInputClassification InputSlotClass;
	uint32 InstanceDataStepRate;
};

struct RHIInputLayoutDesc
{
	const RHIInputElementDesc* pInputElementDescs;
	uint32 NumElements;
};

struct RHICachedPipelineState
{
	const void* pCachedBlob;
	size_t CachedBlobSizeInBytes;
};

struct RHIGraphicsPipelineStateDesc
{
	RHIRootSignature* pRootSignature;
	RHIShaderBytecode* VS;
	RHIShaderBytecode* PS;
	RHIShaderBytecode* DS;
	RHIShaderBytecode* HS;
	RHIShaderBytecode* GS;
	RHIStreamOutputDesc StreamOutput;
	RHIBlendDesc BlendState;
	uint32 SampleMask;
	RHIRasterizerDesc RasterizerState;
	RHIDepthStencilDesc DepthStencilState;
	RHIInputLayoutDesc InputLayout;
	ERHIIndexBufferStripCutValue IBStripCutValue;
	ERHIPrimitiveTopologyType PrimitiveTopologyType;
	uint32 NumRenderTargets;
	ERHIPixelFormat RTVFormats[8];
	ERHIPixelFormat DSVFormat;
	RHISampleDesc SampleDesc;
	uint32 NodeMask;
	RHICachedPipelineState CachedPSO;
	ERHIPipelineStateFlags Flags;
};

struct RHIHeapProperties
{
	ERHIHeapType Type;
	ERHICPUPageProperty CPUPageProperty;
	ERHIMemoryPool MemoryPoolPreference;
	uint32 CreationNodeMask;
	uint32 VisibleNodeMask;
};

struct RHIResourceDesc
{
	ERHIResourceDimension Dimension;
	uint64 Alignment;
	uint64 Width;
	uint32 Height;
	uint16 DepthOrArraySize;
	uint16 MipLevels;
	ERHIPixelFormat Format;
	RHISampleDesc SampleDesc;
	ERHITextureLayout Layout;
	ERHIResourceFlags Flags;
};