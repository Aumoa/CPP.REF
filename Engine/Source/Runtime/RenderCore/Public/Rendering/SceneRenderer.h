// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

namespace Ayla
{
	struct SceneView;
	class RenderTexture;
	class CommandBuffer;
	class RenderPass;

	class RENDERCORE_API SceneRenderer : public NonCopyable
	{
	private:
		std::vector<RenderPass*> m_Passes;

	public:
		SceneRenderer();
		SceneRenderer(SceneRenderer&&) = delete;
		virtual ~SceneRenderer() noexcept;

		void AddPass(RenderPass* pass);

		virtual void Render(CommandBuffer* commandBuffer, const SceneView& view);
	};
}