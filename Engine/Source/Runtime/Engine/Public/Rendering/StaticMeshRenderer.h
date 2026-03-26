// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/Renderer.h"
#include "StaticMeshRenderer.gen.h"

namespace Ayla
{
	class StaticMesh;

	ACLASS()
	class ENGINE_API StaticMeshRenderer : public Renderer
	{
		GENERATED_BODY()

	private:
		SharedPtr<StaticMesh> m_Mesh;

	public:
		ACONSTRUCTOR()
		StaticMeshRenderer();
		virtual ~StaticMeshRenderer() noexcept override;

		AFUNCTION()
		void SetMesh(SharedPtr<StaticMesh> mesh);

	protected:
		virtual std::shared_ptr<RendererProxy> CreateProxy() override;
	};
}