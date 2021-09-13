// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Scene/Scene.h"
#include "Scene/PrimitiveSceneProxy.h"
#include "Scene/SceneVisibility.h"
#include "Scene/SceneRenderer.h"
#include "Components/PrimitiveComponent.h"
#include "Materials/Material.h"

SScene::SScene(World* worldOwner, SRHIDevice* device) : Super()
	, _world(worldOwner)
	, _device(device)
{
	_localPlayerView = NewObject<SSceneVisibility>(this);
}

SScene::~SScene()
{
}

void SScene::UpdateScene(std::chrono::duration<float> elapsedTime)
{
	for (auto& proxy : _primitives)
	{
		if (proxy != nullptr)
		{
			if (proxy->bRenderStateDirty)
			{
				// Replace primitive scene proxy to re-created instance with same primitive id.
				proxy = RecreateRenderSceneProxy(proxy);
			}

			if (proxy == nullptr)
			{
				continue;
			}

			SPrimitiveComponent* component = proxy->GetComponent();
			proxy->UpdateTransform_GameThread(component->GetComponentTransform());
		}
	}
}

void SScene::InitViews(const MinimalViewInfo& localPlayerView)
{
	_localPlayerView->CalcVisibility(localPlayerView);

	// Reset material slots.
	for (auto& it : _renderQueue)
	{
		it.second.clear();
	}

	// Collect primitive materials.
	size_t renderers = 0;
	std::set<SRHIShader*> checks;
	for (SPrimitiveSceneProxy*& proxy : _primitives)
	{
		if (proxy != nullptr)
		{
			for (auto& batch : proxy->MeshBatches)
			{
				for (auto& materialSlot : batch.MaterialSlots)
				{
					if (materialSlot != nullptr)
					{
						std::map<size_t, std::vector<SRHIShader*>>::iterator it;

						size_t queueNumber = 0;
						switch (materialSlot->GetBlendMode())
						{
						case EMaterialBlendMode::Opaque:
							queueNumber = RenderQueue_Opaque;
							break;
						case EMaterialBlendMode::Masked:
							queueNumber = RenderQueue_Masked;
							break;
						case EMaterialBlendMode::Transparent:
							queueNumber = RenderQueue_Transparent;
							break;
						}

						if (!ensureMsgf(queueNumber != 0, L"Render Queue Number is not setted."))
						{
							continue;
						}

						it = _renderQueue.find(queueNumber);
						if (it == _renderQueue.end())
						{
							it = _renderQueue.emplace_hint(it, queueNumber, std::vector<SRHIShader*>());
						}

						if (auto ck = checks.find(materialSlot->GetShader()); ck == checks.end())
						{
							checks.emplace_hint(ck, materialSlot->GetShader());
							it->second.emplace_back(materialSlot->GetShader());
							renderers += 1;
						}
					}
				}
			}
		}
	}

	_renderers.clear();
	_renderers.reserve(renderers);

	// Enqueue renderer.
	for (auto& it : _renderQueue)
	{
		for (auto& slots : it.second)
		{
			SSceneRenderer& renderer = _renderers.emplace_back(this, (SRHIShader*)slots);
			renderer.CollectPrimitives(_localPlayerView);
		}
	}
}

void SScene::RenderScene(SRHIDeviceContext* dc)
{
	for (auto& renderer : _renderers)
	{
		renderer.RenderScene(dc);
	}
}

int64 SScene::AddPrimitive(SPrimitiveSceneProxy* proxy)
{
	// Change proxy's outer for manage lifecycle with render thread.
	if (proxy->GetOuter() != this)
	{
		proxy->SetOuter(this);
	}

	if (_spaces.empty())
	{
		int64 id = (int64)_primitives.size();
		_primitives.emplace_back(proxy);
		return id;
	}
	else
	{
		int64 id = (int64)_spaces.front();
		_spaces.pop();
		_primitives[id] = proxy;
		return id;
	}
}

void SScene::RemovePrimitive(int64 primitiveId)
{
	// PrimitiveId < 0 ==> Invalid parameter.
	if (primitiveId < 0)
	{
		return;
	}

	// PrimitiveId >= NumPrimitives ==> Invalid parameter.
	if ((size_t)primitiveId >= _primitives.size())
	{
		return;
	}

	// Primitives[PrimitiveId] == nullptr ==> Already removed.
	if (_primitives[primitiveId] == nullptr)
	{
		return;
	}

	DestroySubobject(_primitives[primitiveId]);
	_primitives[primitiveId] = nullptr;
	_spaces.emplace(primitiveId);
}

SPrimitiveSceneProxy* SScene::RecreateRenderSceneProxy(SPrimitiveSceneProxy* proxy)
{
	SPrimitiveComponent* component = proxy->GetComponent();
	int64 primitiveId = proxy->PrimitiveId;
	DestroySubobject(proxy);
	proxy = component->CreateSceneProxy();
	if (proxy == nullptr)
	{
		_primitives[primitiveId] = nullptr;
		_spaces.emplace(primitiveId);
	}
	else
	{
		// Remove previous and attach with same primitive id.
		proxy->PrimitiveId = primitiveId;
		component->SceneProxy = proxy;
		_primitives[primitiveId] = proxy;
	}
	return proxy;
}