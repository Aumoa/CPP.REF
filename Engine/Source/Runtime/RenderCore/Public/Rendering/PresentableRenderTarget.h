// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/RenderTexture.h"

namespace Ayla
{
	class CommandBuffer;
	class PresentableRenderTarget;

	class PresentableFrame
	{
	private:
		PresentableRenderTarget* m_Target = nullptr;
		RenderTexture* m_RenderTexture = nullptr;

	public:
		constexpr PresentableFrame() noexcept = default;
		constexpr PresentableFrame(PresentableRenderTarget* target, RenderTexture* renderTexture) noexcept
			: m_Target(target)
			, m_RenderTexture(renderTexture)
		{
		}

		constexpr RenderTexture* GetRenderTexture() const noexcept
		{
			return m_RenderTexture;
		}

		Vector2N GetSize() const
		{
			check(m_RenderTexture != nullptr);
			return m_RenderTexture->GetSize();
		}

		void Present(CommandBuffer* commandBuffer) const;

		constexpr explicit operator bool() const noexcept
		{
			return m_Target != nullptr && m_RenderTexture != nullptr;
		}
	};

	class PresentableRenderTarget
	{
	public:
		virtual ~PresentableRenderTarget() noexcept = default;

		virtual Vector2N GetSize() const = 0;
		virtual PresentableFrame AcquireFrame(CommandBuffer* commandBuffer) = 0;
		virtual void Present(CommandBuffer* commandBuffer) = 0;
		virtual void Invalidate() = 0;
	};

	inline void PresentableFrame::Present(CommandBuffer* commandBuffer) const
	{
		check(m_Target != nullptr);
		m_Target->Present(commandBuffer);
	}
}
