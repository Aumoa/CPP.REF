// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SceneComponent.h"

class SPrimitiveSceneProxy;

class GAME_API SPrimitiveComponent : public SSceneComponent
{
	GENERATED_BODY(SPrimitiveComponent)

private:
	uint8 _bHiddenInGame : 1 = false;

public:
	SPrimitiveComponent();

	virtual SPrimitiveSceneProxy* CreateSceneProxy() { return nullptr; }
	SPrimitiveSceneProxy* SceneProxy = nullptr;

	void SetHiddenInGame(bool bHidden);
	inline bool IsHiddenInGame() const { return _bHiddenInGame; }

	virtual void MarkRenderStateDirty() override;
};