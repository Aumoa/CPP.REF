// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Graphics.h"  // TODO: Move function implementation of reflection to source code
#include "Rendering/Mesh.h"
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

	public:
		ACONSTRUCTOR()
		StaticMesh(SharedPtr<Graphics> graphics);
		virtual ~StaticMesh() noexcept override;
	};
}