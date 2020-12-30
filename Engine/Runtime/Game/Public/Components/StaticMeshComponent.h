// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "MeshComponent.h"

class GAME_API StaticMeshComponent : public MeshComponent
{
public:
	using Super = MeshComponent;
	using This = StaticMeshComponent;

public:
	StaticMeshComponent();
	~StaticMeshComponent() override;

	TRefPtr<PrimitiveSceneProxy> CreateSceneProxy() override;
};