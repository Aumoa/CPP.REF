// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Transform.h"

struct MinimalViewInfo;
class GCameraComponent;

class GAME_API GPlayerCameraManager : public GActorComponent
{
public:
	using Super = GActorComponent;
	using This = GPlayerCameraManager;

private:
	mutable bool bPrintNoCameraWarning : 1;
	GCameraComponent* pawnCamera;
	Transform lastUpdatedTransform;

public:
	GPlayerCameraManager();
	~GPlayerCameraManager() override;

	void CalcCameraView(MinimalViewInfo& outViewInfo) const;
	void UpdateCameraComponent();

private:
	void PrintNoCameraWarning(TRefPtr<String> message) const;
};