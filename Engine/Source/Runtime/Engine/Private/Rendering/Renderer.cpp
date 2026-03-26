// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Rendering/Renderer.h"
#include "Rendering/RendererProxy.h"
#include "GameContext.h"
#include "SceneManagement/SceneManager.h"
#include "Rendering/RenderScene.h"

namespace Ayla
{
	Renderer::Renderer()
	{
	}

	Renderer::~Renderer() noexcept
	{
	}

	std::shared_ptr<RendererProxy> Renderer::UpdateAndGetProxy()
	{
		if (!IsEnabled())
		{
			return nullptr;
		}

		if (m_RenderStateDirty)
		{
			m_Proxy = CreateProxy();
			m_RenderStateDirty = false;
		}

		return m_Proxy;
	}

	void Renderer::OnEnable_Implementation()
	{
		Super::OnEnable_Implementation();
		MarkRenderStateDirty();
	}

	void Renderer::OnDisable_Implementation()
	{
		Super::OnDisable_Implementation();
		auto* sm = GameContext::GetSceneManager();
		sm->UnmarkRendererDirty(this);
	}

	void Renderer::MarkRenderStateDirty()
	{
		m_RenderStateDirty = true;
		m_Proxy.reset();

		auto* sm = GameContext::GetSceneManager();
		sm->MarkRendererDirty(this);
	}

	std::shared_ptr<RendererProxy> Renderer::CreateProxy()
	{
		return nullptr;
	}
}