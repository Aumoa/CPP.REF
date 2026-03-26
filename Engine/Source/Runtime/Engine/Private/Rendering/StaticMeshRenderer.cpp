// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Rendering/StaticMeshRenderer.h"
#include "Rendering/StaticMesh.h"
#include "Rendering/StaticMeshRendererProxy.h"

namespace Ayla
{
	StaticMeshRenderer::StaticMeshRenderer()
	{
	}

	StaticMeshRenderer::~StaticMeshRenderer() noexcept
	{
	}

	void StaticMeshRenderer::SetMesh(SharedPtr<StaticMesh> mesh)
	{
		if (m_Mesh != mesh)
		{
			m_Mesh = mesh;
			MarkRenderStateDirty();
		}
	}

	std::shared_ptr<RendererProxy> StaticMeshRenderer::CreateProxy()
	{
		auto proxy = std::make_shared<StaticMeshRendererProxy>();
		return proxy;
	}
}