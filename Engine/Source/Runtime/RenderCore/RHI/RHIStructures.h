// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHIEnums.h"

class RHIResource;

#pragma pack(push, 4)

/// <summary>
/// Represent viewport area.
/// </summary>
struct RHIViewport
{
	float TopLeftX = 0;
	float TopLeftY = 0;
	float Width = 0;
	float Height = 0;
	float MinDepth = 0;
	float MaxDepth = 0;
};

/// <summary>
/// Represent viewport scissor rect.
/// </summary>
struct RHIScissorRect
{
	int32 Left = 0;
	int32 Top = 0;
	int32 Right = 0;
	int32 Bottom = 0;
};

/// <summary>
/// Represent transition resource state information.
/// </summary>
struct RHITransitionBarrier
{
	RHIResource* Resource = nullptr;
	ERHIResourceStates StateBefore = ERHIResourceStates::Common;
	ERHIResourceStates StateAfter = ERHIResourceStates::Common;
	uint32 SubresourceIndex = 0;
};

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

#pragma pack(pop)