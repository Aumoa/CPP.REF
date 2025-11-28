// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/Mesh.h"
#include "Numerics/VectorInterface/Vector.h"
#include "StaticMesh.gen.h"

namespace Ayla
{
	class Buffer;

	ACLASS()
	class ENGINE_API StaticMesh : public Mesh
	{
		GENERATED_BODY()

	private:
		SharedPtr<Buffer> m_VertexBuffer;
		SharedPtr<Buffer> m_IndexBuffer;

	public:
		ACONSTRUCTOR()
		StaticMesh(SharedPtr<Graphics> graphics);
		virtual ~StaticMesh() noexcept override;

		void SetVertices(std::span<const Vector3D> vertices);
		void SetIndices(std::span<const int32> indices);
	};
}