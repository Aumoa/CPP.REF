// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Transform.h"

struct MinimalViewInfo;
class CameraComponent;

class GAME_API PlayerCameraManager : public ActorComponent
{
public:
	using Super = ActorComponent;
	using This = PlayerCameraManager;

private:
	mutable bool bPrintNoCameraWarning : 1;
	CameraComponent* pawnCamera;
	Transform lastUpdatedTransform;

public:
	PlayerCameraManager();
	~PlayerCameraManager() override;

	void CalcCameraView(MinimalViewInfo& outViewInfo) const;
	void UpdateCameraComponent();

private:
	void PrintNoCameraWarning(TRefPtr<String> message) const;
};