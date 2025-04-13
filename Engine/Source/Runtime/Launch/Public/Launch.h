// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericApplication.h"
#include "Engine.h"
#include "Launch.gen.h"

namespace Ayla
{
	ACLASS()
	class LAUNCH_API Launch : public Object
	{
		GENERATED_BODY()

	private:
		APROPERTY()
		PPtr<GenericApplication> GenericApp;
		APROPERTY()
		PPtr<Engine> Engine;

	protected:
		Launch();

	public:
		AFUNCTION()
		virtual int32 StartApplication();

		virtual void* GetApplicationPointer() = 0;
	};
}