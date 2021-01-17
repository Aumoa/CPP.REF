// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "THAPI.h"
#include "GameMinimal.h"
#include "Framework/PlayerController.h"

class TH_API ATHPlayerController : public APlayerController
{
	using Super = APlayerController;
	using This = ATHPlayerController;

public:
	ATHPlayerController();
	~ATHPlayerController() override;

	void BeginPlay() override;
	void Tick(Seconds deltaTime) override;
};