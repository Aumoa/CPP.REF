// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "THAPI.h"
#include "GameMinimal.h"
#include "Framework/PlayerController.h"

class TickScheduler;

class TH_API ATHPlayerController : public APlayerController
{
public:
	using Super = APlayerController;

private:
	TRefPtr<TickScheduler> diag_tick;

public:
	ATHPlayerController();
	~ATHPlayerController() override;

	void BeginPlay() override;
	void Tick(Seconds deltaTime) override;
};