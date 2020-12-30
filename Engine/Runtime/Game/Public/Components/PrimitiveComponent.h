// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "SceneComponent.h"

class PrimitiveSceneProxy;

class GAME_API PrimitiveComponent : public SceneComponent
{
public:
	using Super = SceneComponent;
	using This = PrimitiveComponent;

private:
	TRefPtr<PrimitiveSceneProxy> sceneProxy;
	bool bHasDirtyMark : 1;

public:
	PrimitiveComponent();
	~PrimitiveComponent() override;

	virtual TRefPtr<PrimitiveSceneProxy> CreateSceneProxy();
	PrimitiveSceneProxy* GetSceneProxy() const;

	void SetDirtyMark();
	bool HasDirtyMark() const;
	virtual void ResolveDirtyState();
};