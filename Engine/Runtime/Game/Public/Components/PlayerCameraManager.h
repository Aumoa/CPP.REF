// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

struct MinimalViewInfo;
class CameraComponent;

class GAME_API PlayerCameraManager : public ActorComponent
{
public:
	using Super = ActorComponent;
	using This = PlayerCameraManager;

private:
	CameraComponent* pawnCamera;

public:
	PlayerCameraManager();
	~PlayerCameraManager() override;

	void CalcCameraView(MinimalViewInfo& outViewInfo) const;
	void UpdateCameraComponent();
};