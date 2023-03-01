// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class APPLICATIONCORE_API NGenericSplash
{
	NGenericSplash() = delete;

public:
	static void Show();
	static void Hide();

	static void SetSplashText(String InText);
};