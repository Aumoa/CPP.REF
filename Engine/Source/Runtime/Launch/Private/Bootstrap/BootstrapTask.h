// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericSplash.h"

class NBootstrapTask
{
	const String ContextName;
	const float TotalAmount;
	float CurrentAmount = 0;

public:
	NBootstrapTask(String InName, float Amount)
		: ContextName(InName)
		, TotalAmount(Amount)
	{
		UpdateSplashWindow();
	}

	void Step(float InAmount)
	{
		float Previous = CurrentAmount;
		CurrentAmount += InAmount;
		if (CurrentAmount > TotalAmount)
		{
			CurrentAmount = TotalAmount;
		}

		if ((int32)Previous == (int32)CurrentAmount)
		{
			return;
		}

		UpdateSplashWindow();
	}

private:
	void UpdateSplashWindow()
	{
		NGenericSplash::SetSplashText(String::Format(TEXT("Initializing... - {} ({}%)"), ContextName, (int32)CurrentAmount));
	}
};