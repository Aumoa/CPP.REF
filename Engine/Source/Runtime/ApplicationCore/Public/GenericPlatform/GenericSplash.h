// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

namespace Ayla
{
	class APPLICATIONCORE_API GenericSplash : virtual public Object
	{
		GenericSplash() = delete;

	public:
		static void Show();
		static void Hide();

		static void SetSplashText(String InText);
	};
}