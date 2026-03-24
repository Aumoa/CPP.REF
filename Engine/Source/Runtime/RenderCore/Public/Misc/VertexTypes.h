// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/VectorInterface/Vector.h"
#include "Misc/PrimitiveVertex.h"

namespace Ayla
{
	/// <summary>
	/// Vertex type with position only.
	/// Useful for shadow map rendering and depth prepass.
	/// </summary>
	struct PositionVertex
	{
		Vector3F Position;
	};

	/// <summary>
	/// Vertex type with position and a single color.
	/// Useful for debug rendering and simple colored geometry.
	/// </summary>
	struct PositionColorVertex
	{
		Vector3F Position;
		Vector4F Color;
	};

	/// <summary>
	/// Vertex type with position and normal.
	/// Useful for simple diffuse lighting without texture mapping.
	/// </summary>
	struct PositionNormalVertex
	{
		Vector3F Position;
		Vector3F Normal;
	};

	/// <summary>
	/// Vertex type with position, normal, tangent, bitangent and texture coordinate.
	/// Suitable for PBR rendering with normal mapping.
	/// </summary>
	struct FullVertex
	{
		Vector3F Position;
		Vector3F Normal;
		Vector3F Tangent;
		Vector3F Bitangent;
		Vector2F Tex;
	};

	// PrimitiveVertex (Position + Normal + Tex) is defined in PrimitiveVertex.h
	// and serves as the default vertex type for geometry generation.
}
