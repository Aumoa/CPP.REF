// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.RenderCore:RHIStructures;

import SC.Runtime.Core;
import :RHIEnums;
import std.core;

export class RHIResource;

/// <summary>
/// Represent viewport area.
/// </summary>
export struct RHIViewport
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
export struct RHIScissorRect
{
	int32 Left = 0;
	int32 Top = 0;
	int32 Right = 0;
	int32 Bottom = 0;
};

/// <summary>
/// Represent transition resource state information.
/// </summary>
export struct RHITransitionBarrier
{
	RHIResource* Resource = nullptr;
	ERHIResourceStates StateBefore = ERHIResourceStates::Common;
	ERHIResourceStates StateAfter = ERHIResourceStates::Common;
	uint32 SubresourceIndex = 0;
};

/// <summary>
/// Represents vertex element using input layout.
/// </summary>
export struct RHIVertexElement
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
export struct RHIVertex
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
	Color Color;
};

/// <summary>
/// Represents vertex buffer view.
/// </summary>
export struct RHIVertexBufferView
{
	uint64 BufferLocation = 0;
	uint32 SizeInBytes = 0;
	uint32 StrideInBytes = 0;
};