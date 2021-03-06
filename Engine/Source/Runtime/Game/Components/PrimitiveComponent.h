// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SceneComponent.h"

class PrimitiveSceneProxy;

class PrimitiveComponent : public SceneComponent
{
public:
	using Super = SceneComponent;

private:
	uint8 _bHiddenInGame : 1 = false;

public:
	PrimitiveComponent();

	virtual PrimitiveSceneProxy* CreateSceneProxy() { return nullptr; }
	PrimitiveSceneProxy* SceneProxy = nullptr;

	void SetHiddenInGame(bool bHidden);
	inline bool IsHiddenInGame() const { return _bHiddenInGame; }

	virtual void MarkRenderStateDirty() override;
};