// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

interface IFrameworkView;
class PaintArgs;
class SLocalPlayer;
class SWindow;

class GAME_API SSlateApplication : implements SObject
{
	GENERATED_BODY(SSlateApplication)

private:
	SLocalPlayer* LocalPlayer = nullptr;
	SWindow* CoreWindow = nullptr;

public:
	SSlateApplication();

	void InitWindow(SLocalPlayer* InLocalPlayer, IFrameworkView* InFrameworkView);

	void Tick(float InDeltaTime);
	void PopulateCommandLists(const PaintArgs& Args);
};