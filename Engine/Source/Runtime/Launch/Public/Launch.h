// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Launch.gen.h"

namespace Ayla
{
	class GenericApplication;
	class Engine;
	class PlatformRenderFeature;

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
		virtual ~Launch() noexcept override;

		AFUNCTION()
		virtual int32 StartApplication();

		virtual void* GetApplicationPointer() = 0;
		virtual RPtr<PlatformRenderFeature> CreatePlatformRenderFeature() = 0;
	};
}