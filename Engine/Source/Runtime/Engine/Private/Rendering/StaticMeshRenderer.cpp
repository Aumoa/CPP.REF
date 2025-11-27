// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Rendering/StaticMeshRenderer.h"
#include "Rendering/StaticMesh.h"

namespace Ayla
{
	StaticMeshRenderer::StaticMeshRenderer()
	{
	}

	void StaticMeshRenderer::SetMesh(SharedPtr<StaticMesh> mesh)
	{
		m_Mesh = mesh;
	}
}