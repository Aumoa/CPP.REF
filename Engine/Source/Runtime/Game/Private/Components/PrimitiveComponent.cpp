// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Components/PrimitiveComponent.h"
#include "Rendering/PrimitiveSceneProxy.h"
#include "PrimitiveComponent.gen.cpp"

PrimitiveComponent::PrimitiveComponent()
{
}

PrimitiveComponent::~PrimitiveComponent() noexcept
{
	check(SceneProxy == nullptr);
}

void PrimitiveComponent::RegisterComponent()
{
	Super::RegisterComponent();
	SceneProxy = CreateSceneProxy();
}

void PrimitiveComponent::UnregisterComponent()
{
	Super::UnregisterComponent();
	SceneProxy = nullptr;
}

void PrimitiveComponent::ResolveRenderState()
{
	if (bMarkRenderStateDirty)
	{
		SceneProxy = CreateSceneProxy();
	}

	bMarkRenderStateDirty = false;
}

void PrimitiveComponent::MarkRenderStateDirty()
{
	bMarkRenderStateDirty = true;
}

PrimitiveSceneProxy* PrimitiveComponent::CreateSceneProxy()
{
	return nullptr;
}