// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SceneComponent.h"

class PrimitiveSceneProxy;

class GAME_API SPrimitiveComponent : public SSceneComponent
{
	GENERATED_BODY(SPrimitiveComponent)

private:
	uint8 _bHiddenInGame : 1 = false;

public:
	SPrimitiveComponent();
	~SPrimitiveComponent() override;

	virtual PrimitiveSceneProxy* CreateSceneProxy() { return nullptr; }
	PrimitiveSceneProxy* SceneProxy = nullptr;

	void SetHiddenInGame(bool bHidden);
	inline bool IsHiddenInGame() { return _bHiddenInGame; }

	virtual void MarkRenderStateDirty() override;
};