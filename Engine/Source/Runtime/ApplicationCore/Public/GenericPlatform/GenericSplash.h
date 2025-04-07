// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericSplash.gen.h"

namespace Ayla
{
	ACLASS()
	class APPLICATIONCORE_API GenericSplash : public Object
	{
		GENERATED_BODY()

	protected:
		GenericSplash() = delete;

	public:
		static void Show();
		static void Hide();

		static void SetSplashText(String InText);
	};
}