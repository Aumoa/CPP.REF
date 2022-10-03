// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Components/PrimitiveComponent.h"
#include "Rendering/PrimitiveSceneProxy.h"
#include "Rendering/PrimitiveSceneInfo.h"
#include "Rendering/RenderScene.h"
#include "WorldCore/World.h"
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

	SceneInfo = new PrimitiveSceneInfo();
	SceneInfo->SceneProxy = CreateSceneProxy();
	SceneProxy = SceneInfo->SceneProxy;
	GetWorld()->GetScene()->AddPrimitive(SceneInfo);
}

void PrimitiveComponent::UnregisterComponent()
{
	if (World* GameWorld = GetWorld(); GameWorld && SceneInfo)
	{
		GameWorld->GetScene()->RemovePrimitive(SceneInfo->GetPrimitiveId());
		SceneInfo = nullptr;
	}

	Super::UnregisterComponent();
}

void PrimitiveComponent::ResolveRenderState()
{
	if (World* GameWorld = GetWorld(); GameWorld && SceneInfo && bMarkRenderStateDirty)
	{
		SceneProxy = CreateSceneProxy();
		GameWorld->GetScene()->UpdatePrimitive(SceneInfo->GetPrimitiveId(), SceneProxy);
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