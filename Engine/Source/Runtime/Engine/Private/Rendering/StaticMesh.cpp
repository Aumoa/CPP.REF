// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Rendering/StaticMesh.h"
#include "Graphics.h"
#include "Buffer.h"

namespace Ayla
{
	StaticMesh::StaticMesh(SharedPtr<Graphics> graphics)
		: Super(std::move(graphics))
		, m_VertexBuffer(GetGraphics()->CreateBuffer(BufferUsage::VertexBuffer))
	{
	}

	StaticMesh::~StaticMesh() noexcept
	{
	}
}