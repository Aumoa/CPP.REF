// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

namespace Ayla
{
	class SceneView;
	class RenderTexture;

	class RENDERCORE_API SceneRenderer : public NonCopyable
	{
	private:
		SharedPtr<RenderTexture> m_OutputTexture;

	protected:
		SceneRenderer(SharedPtr<RenderTexture> outputTexture);
		SceneRenderer(SceneRenderer&&) = delete;

	public:
		virtual ~SceneRenderer() noexcept;

		virtual void Render(const SceneView& view) = 0;

		RenderTexture* GetOutputTexture() const { return m_OutputTexture.Get(); }
	};
}