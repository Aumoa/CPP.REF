// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Scene.h"
#include "PrimitiveSceneProxy.h"
#include "SceneVisibility.h"
#include "Components/PrimitiveComponent.h"

using namespace std::chrono;

Scene::Scene(World* worldOwner, RHIDevice* device) : Super()
	, _world(worldOwner)
	, _device(device)
{
	_localPlayerView = CreateSubobject<SceneVisibility>(this);
}

void Scene::UpdateScene(duration<float> elapsedTime)
{
	for (auto& proxy : _primitives)
	{
		PrimitiveComponent* component = proxy->GetComponent();

		if (proxy->bRenderStateDirty)
		{
			// Replace primitive scene proxy to re-created instance with same primitive id.
			int64 primitiveId = proxy->PrimitiveId;
			DestroySubobject(proxy);
			proxy = component->CreateSceneProxy();
			proxy->PrimitiveId = primitiveId;
		}

		proxy->UpdateTransform_GameThread(component->GetComponentTransform());
	}
}

void Scene::InitViews(const MinimalViewInfo& localPlayerView)
{
	_localPlayerView->CalcVisibility(localPlayerView);
}

int64 Scene::AddPrimitive(PrimitiveSceneProxy* proxy)
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

void Scene::RemovePrimitive(int64 primitiveId)
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

	_primitives[primitiveId] = nullptr;
	_spaces.emplace(primitiveId);
}