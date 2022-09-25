// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SceneComponent.h"
#include "PrimitiveComponent.gen.h"

class PrimitiveSceneProxy;

SCLASS()
class GAME_API PrimitiveComponent : public SceneComponent
{
	GENERATED_BODY()

protected:
	PrimitiveSceneProxy* SceneProxy = nullptr;
	bool bMarkRenderStateDirty = false;

public:
	PrimitiveComponent();
	virtual ~PrimitiveComponent() noexcept override;

	virtual void RegisterComponent() override;
	virtual void UnregisterComponent() override;

	void ResolveRenderState();
	void MarkRenderStateDirty();

protected:
	virtual PrimitiveSceneProxy* CreateSceneProxy();
};